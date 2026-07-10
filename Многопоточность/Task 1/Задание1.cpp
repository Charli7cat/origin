#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

class QueueSimulator {
private:
    std::atomic<int> clientCount;
    int maxClients;
    std::atomic<bool> isOpen;
    std::mutex mtx;
    std::condition_variable cv;

public:
    QueueSimulator(int max) : clientCount(0), maxClients(max), isOpen(true) {}

    void clientThread() {
        std::cout << "Клиентский поток запущен. Максимум клиентов: " << maxClients << std::endl;

        while (isOpen) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::unique_lock<std::mutex> lock(mtx);

            if (clientCount.load() < maxClients) {
                int newCount = clientCount.fetch_add(1) + 1;
                std::cout << "Пришёл новый клиент. Всего клиентов: " << newCount << std::endl;
                cv.notify_one();
            }
            else {
                std::cout << "Очередь переполнена! Клиент ушёл. Текущее количество: "
                    << clientCount.load() << std::endl;
            }
        }
    }

    void operatorThread() {
        std::cout << "Операционист запущен. Начинает обслуживание..." << std::endl;

        while (isOpen) {
            std::unique_lock<std::mutex> lock(mtx);

            while (clientCount.load() == 0 && isOpen) {
                std::cout << "Операционист ждёт клиентов..." << std::endl;
                cv.wait_for(lock, std::chrono::seconds(2), [this]() {
                    return clientCount.load() > 0 || !isOpen;
                    });

                if (!isOpen) break;
            }

            if (!isOpen) break;

            if (clientCount.load() > 0) {
                int newCount = clientCount.fetch_sub(1) - 1;
                std::cout << "Операционист обслужил клиента. Осталось клиентов: "
                    << newCount << std::endl;
            }

            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    void stop() {
        isOpen = false;
        cv.notify_all();
    }

    void run() {
        std::thread client(&QueueSimulator::clientThread, this);
        std::thread operatorWorker(&QueueSimulator::operatorThread, this);

        std::this_thread::sleep_for(std::chrono::seconds(30));

        stop();

        client.join();
        operatorWorker.join();

        std::cout << "Симуляция завершена. Осталось клиентов: " << clientCount.load() << std::endl;
    }
};

int main() {
    std::cout << "=== Симуляция очереди в окошко ===" << std::endl;
    std::cout << "Введите максимальное количество клиентов в очереди: ";

    int maxClients;
    std::cin >> maxClients;

    if (maxClients <= 0) {
        std::cout << "Некорректное значение. Установлено значение по умолчанию: 5" << std::endl;
        maxClients = 5;
    }

    QueueSimulator simulator(maxClients);
    simulator.run();

    return 0;
}