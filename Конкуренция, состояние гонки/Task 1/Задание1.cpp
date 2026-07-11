#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <iomanip>
#include <vector>
#include <string>

class AtomicQueueSimulator {
private:
    std::atomic<int> clientCount;
    int maxClients;
    std::atomic<bool> isOpen;
    std::atomic<int> totalServed;
    std::atomic<int> totalRejected;

    enum MemoryModel {
        SEQUENTIAL_CONSISTENCY,
        ACQUIRE_RELEASE,
        RELAXED
    };

    MemoryModel currentModel;
    std::string modelName;

    int atomicIncrement(std::atomic<int>& counter, MemoryModel model) {
        switch (model) {
        case SEQUENTIAL_CONSISTENCY:
            return counter.fetch_add(1, std::memory_order_seq_cst);
        case ACQUIRE_RELEASE:
            return counter.fetch_add(1, std::memory_order_acq_rel);
        case RELAXED:
            return counter.fetch_add(1, std::memory_order_relaxed);
        default:
            return counter.fetch_add(1, std::memory_order_seq_cst);
        }
    }

    int atomicDecrement(std::atomic<int>& counter, MemoryModel model) {
        switch (model) {
        case SEQUENTIAL_CONSISTENCY:
            return counter.fetch_sub(1, std::memory_order_seq_cst);
        case ACQUIRE_RELEASE:
            return counter.fetch_sub(1, std::memory_order_acq_rel);
        case RELAXED:
            return counter.fetch_sub(1, std::memory_order_relaxed);
        default:
            return counter.fetch_sub(1, std::memory_order_seq_cst);
        }
    }

    int atomicLoad(const std::atomic<int>& counter, MemoryModel model) {
        switch (model) {
        case SEQUENTIAL_CONSISTENCY:
            return counter.load(std::memory_order_seq_cst);
        case ACQUIRE_RELEASE:
            return counter.load(std::memory_order_acquire);
        case RELAXED:
            return counter.load(std::memory_order_relaxed);
        default:
            return counter.load(std::memory_order_seq_cst);
        }
    }

    void atomicStore(std::atomic<bool>& flag, bool value, MemoryModel model) {
        switch (model) {
        case SEQUENTIAL_CONSISTENCY:
            flag.store(value, std::memory_order_seq_cst);
            break;
        case ACQUIRE_RELEASE:
            flag.store(value, std::memory_order_release);
            break;
        case RELAXED:
            flag.store(value, std::memory_order_relaxed);
            break;
        default:
            flag.store(value, std::memory_order_seq_cst);
        }
    }

public:
    AtomicQueueSimulator(int max)
        : clientCount(0), maxClients(max), isOpen(true),
        totalServed(0), totalRejected(0),
        currentModel(SEQUENTIAL_CONSISTENCY), modelName("Sequential Consistency") {
    }

    void setMemoryModel(int model) {
        switch (model) {
        case 1:
            currentModel = SEQUENTIAL_CONSISTENCY;
            modelName = "Sequential Consistency (seq_cst)";
            break;
        case 2:
            currentModel = ACQUIRE_RELEASE;
            modelName = "Acquire-Release (acq_rel)";
            break;
        case 3:
            currentModel = RELAXED;
            modelName = "Relaxed (relaxed)";
            break;
        default:
            currentModel = SEQUENTIAL_CONSISTENCY;
            modelName = "Sequential Consistency (seq_cst)";
        }
    }

    void clientThread(int id) {
        while (atomicLoad(isOpen, currentModel)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            int current = atomicLoad(clientCount, currentModel);

            if (current < maxClients) {
                int newCount = atomicIncrement(clientCount, currentModel) + 1;
                std::cout << "[Клиент " << id << "] Пришёл. Всего: " << newCount << std::endl;
            }
            else {
                atomicIncrement(totalRejected, currentModel);
                std::cout << "[Клиент " << id << "] Очередь полна! Отказ. Всего: "
                    << current << std::endl;
            }
        }
    }

    void operatorThread(int id) {
        while (atomicLoad(isOpen, currentModel) || atomicLoad(clientCount, currentModel) > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            if (atomicLoad(clientCount, currentModel) > 0) {
                int newCount = atomicDecrement(clientCount, currentModel) - 1;
                atomicIncrement(totalServed, currentModel);
                std::cout << "[Операционист " << id << "] Обслужил. Осталось: "
                    << newCount << std::endl;
            }
            else if (atomicLoad(isOpen, currentModel)) {
                std::cout << "[Операционист " << id << "] Нет клиентов, жду..." << std::endl;
            }
        }
    }

    void run(int numClients, int numOperators, int durationSeconds) {
        std::cout << "\n=== ТЕСТИРОВАНИЕ МОДЕЛИ ПАМЯТИ: " << modelName << " ===" << std::endl;
        std::cout << "Клиентов: " << numClients << ", Операционистов: " << numOperators << std::endl;
        std::cout << "Максимум в очереди: " << maxClients << std::endl;
        std::cout << "Длительность: " << durationSeconds << " секунд" << std::endl;
        std::cout << std::endl;

        // Сброс счётчиков
        clientCount.store(0);
        totalServed.store(0);
        totalRejected.store(0);
        atomicStore(isOpen, true, currentModel);

        std::vector<std::thread> threads;

        for (int i = 0; i < numClients; ++i) {
            threads.emplace_back(&AtomicQueueSimulator::clientThread, this, i);
        }

        for (int i = 0; i < numOperators; ++i) {
            threads.emplace_back(&AtomicQueueSimulator::operatorThread, this, i);
        }

        std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));

        atomicStore(isOpen, false, currentModel);

        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "\n=== СТАТИСТИКА ===" << std::endl;
        std::cout << "Всего обслужено: " << atomicLoad(totalServed, currentModel) << std::endl;
        std::cout << "Всего отклонено: " << atomicLoad(totalRejected, currentModel) << std::endl;
        std::cout << "Осталось в очереди: " << atomicLoad(clientCount, currentModel) << std::endl;
        std::cout << std::endl;
    }

    void runAllTests() {
        std::cout << "=== ТЕСТИРОВАНИЕ РАЗЛИЧНЫХ МОДЕЛЕЙ ПАМЯТИ ===" << std::endl;
        std::cout << "Доступное количество аппаратных ядер: "
            << std::thread::hardware_concurrency() << std::endl << std::endl;

        std::vector<int> models = { 1, 2, 3 };
        std::vector<std::string> modelNames = {
            "Sequential Consistency",
            "Acquire-Release",
            "Relaxed"
        };

        for (int model : models) {
            setMemoryModel(model);
            run(3, 2, 5);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "\n=== ТЕСТИРОВАНИЕ С РАЗНЫМИ НАГРУЗКАМИ ===" << std::endl;
        std::cout << "Используется модель: " << modelName << std::endl << std::endl;

        struct TestConfig {
            int clients;
            int operators;
            int duration;
            std::string description;
        };

        std::vector<TestConfig> configs = {
            {2, 1, 3, "Мало клиентов, один операционист"},
            {5, 2, 5, "Средняя нагрузка"},
            {10, 3, 8, "Высокая нагрузка"},
            {20, 5, 10, "Очень высокая нагрузка"}
        };

        for (const auto& config : configs) {
            std::cout << "Конфигурация: " << config.description << std::endl;
            run(config.clients, config.operators, config.duration);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void runComparativeTest() {
        std::cout << "\n=== СРАВНИТЕЛЬНЫЙ ТЕСТ РАЗНЫХ МОДЕЛЕЙ ПАМЯТИ ===" << std::endl;
        std::cout << "Фиксированная конфигурация: 5 клиентов, 3 операциониста, 10 секунд" << std::endl;
        std::cout << "Максимум в очереди: 10" << std::endl << std::endl;

        std::cout << std::setw(25) << "Модель памяти"
            << std::setw(15) << "Обслужено"
            << std::setw(15) << "Отклонено"
            << std::setw(15) << "Осталось" << std::endl;
        std::cout << std::string(70, '-') << std::endl;

        struct Result {
            std::string model;
            int served;
            int rejected;
            int remaining;
        };

        std::vector<Result> results;

        for (int model : {1, 2, 3}) {
            setMemoryModel(model);

            clientCount.store(0);
            totalServed.store(0);
            totalRejected.store(0);
            atomicStore(isOpen, true, currentModel);

            std::vector<std::thread> threads;

            for (int i = 0; i < 5; ++i) {
                threads.emplace_back(&AtomicQueueSimulator::clientThread, this, i);
            }

            for (int i = 0; i < 3; ++i) {
                threads.emplace_back(&AtomicQueueSimulator::operatorThread, this, i);
            }

            std::this_thread::sleep_for(std::chrono::seconds(10));

            atomicStore(isOpen, false, currentModel);

            for (auto& thread : threads) {
                thread.join();
            }

            results.push_back({
                modelName,
                totalServed.load(),
                totalRejected.load(),
                clientCount.load()
                });

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        for (const auto& result : results) {
            std::cout << std::setw(25) << result.model
                << std::setw(15) << result.served
                << std::setw(15) << result.rejected
                << std::setw(15) << result.remaining << std::endl;
        }

        std::cout << "\n=== АНАЛИЗ ===" << std::endl;
        std::cout << "1. Sequential Consistency - наиболее строгий, гарантирует глобальный порядок" << std::endl;
        std::cout << "   - Самый безопасный, но потенциально медленный" << std::endl;
        std::cout << "   - Подходит для критических операций" << std::endl;
        std::cout << std::endl;
        std::cout << "2. Acquire-Release - баланс между производительностью и гарантиями" << std::endl;
        std::cout << "   - Обеспечивает синхронизацию между потоками" << std::endl;
        std::cout << "   - Хороший выбор для большинства случаев" << std::endl;
        std::cout << std::endl;
        std::cout << "3. Relaxed - максимальная производительность, минимальные гарантии" << std::endl;
        std::cout << "   - Не гарантирует порядка операций между потоками" << std::endl;
        std::cout << "   - Может привести к непредсказуемым результатам" << std::endl;
        std::cout << "   - Использовать только когда порядок не важен" << std::endl;
        std::cout << std::endl;
        std::cout << "Рекомендация: для данной задачи лучше использовать Acquire-Release" << std::endl;
        std::cout << "как компромисс между производительностью и корректностью." << std::endl;
    }
};

int main() {
    std::cout << "=== АТОМАРНАЯ ИМИТАЦИЯ ОЧЕРЕДИ С РАЗНЫМИ МОДЕЛЯМИ ПАМЯТИ ===" << std::endl;
    std::cout << std::endl;

    std::cout << "Выберите режим работы:" << std::endl;
    std::cout << "1. Полное тестирование всех моделей памяти" << std::endl;
    std::cout << "2. Сравнительный тест моделей памяти" << std::endl;
    std::cout << "3. Ручной режим (с выбором модели)" << std::endl;
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;

    int maxClients = 10;
    std::cout << "Введите максимальное количество клиентов в очереди (по умолчанию 10): ";
    std::cin >> maxClients;
    if (maxClients <= 0) maxClients = 10;

    AtomicQueueSimulator simulator(maxClients);

    switch (choice) {
    case 1:
        simulator.runAllTests();
        break;
    case 2:
        simulator.runComparativeTest();
        break;
    case 3: {
        std::cout << "Выберите модель памяти:" << std::endl;
        std::cout << "1. Sequential Consistency (seq_cst)" << std::endl;
        std::cout << "2. Acquire-Release (acq_rel)" << std::endl;
        std::cout << "3. Relaxed (relaxed)" << std::endl;
        std::cout << "Ваш выбор: ";
        int model;
        std::cin >> model;
        simulator.setMemoryModel(model);

        int clients, operators, duration;
        std::cout << "Введите количество клиентов: ";
        std::cin >> clients;
        std::cout << "Введите количество операционистов: ";
        std::cin >> operators;
        std::cout << "Введите длительность работы (сек): ";
        std::cin >> duration;

        simulator.run(clients, operators, duration);
        break;
    }
    default:
        std::cout << "Неверный выбор. Запускаем полное тестирование." << std::endl;
        simulator.runAllTests();
    }

    return 0;
}