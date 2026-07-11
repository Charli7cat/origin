#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>
#include <sstream>

class Data {
private:
    int id;
    double value;
    std::string name;
    mutable std::mutex mtx;

public:
    Data() : id(0), value(0.0), name("Default") {}

    Data(int id, double value, const std::string& name)
        : id(id), value(value), name(name) {
    }

    int getId() const {
        std::lock_guard<std::mutex> lock(mtx);
        return id;
    }

    double getValue() const {
        std::lock_guard<std::mutex> lock(mtx);
        return value;
    }

    std::string getName() const {
        std::lock_guard<std::mutex> lock(mtx);
        return name;
    }

    void setId(int newId) {
        std::lock_guard<std::mutex> lock(mtx);
        id = newId;
    }

    void setValue(double newValue) {
        std::lock_guard<std::mutex> lock(mtx);
        value = newValue;
    }

    void setName(const std::string& newName) {
        std::lock_guard<std::mutex> lock(mtx);
        name = newName;
    }

    void swapData(Data& other) {
        std::swap(id, other.id);
        std::swap(value, other.value);
        std::swap(name, other.name);
    }

    void print() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Data{id=" << id
            << ", value=" << std::fixed << std::setprecision(2) << value
            << ", name=\"" << name << "\"}";
    }

    std::mutex& getMutex() const {
        return mtx;
    }
};

void swapWithLock(Data& a, Data& b) {
    std::cout << "[lock] Начало обмена" << std::endl;

    std::mutex& mtx_a = a.getMutex();
    std::mutex& mtx_b = b.getMutex();

    std::lock(mtx_a, mtx_b);

    std::lock_guard<std::mutex> lock_a(mtx_a, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(mtx_b, std::adopt_lock);

    a.swapData(b);

    std::cout << "[lock] Обмен завершён" << std::endl;
}

void swapWithScopedLock(Data& a, Data& b) {
    std::cout << "[scoped_lock] Начало обмена" << std::endl;

    std::scoped_lock lock(a.getMutex(), b.getMutex());

    a.swapData(b);

    std::cout << "[scoped_lock] Обмен завершён" << std::endl;
}

void swapWithUniqueLock(Data& a, Data& b) {
    std::cout << "[unique_lock] Начало обмена" << std::endl;

    std::unique_lock<std::mutex> lock_a(a.getMutex(), std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.getMutex(), std::defer_lock);

    std::lock(lock_a, lock_b);

    a.swapData(b);

    std::cout << "[unique_lock] Обмен завершён" << std::endl;
}

void swapWithScopedLockSafe(Data& a, Data& b) {
    if (&a == &b) {
        std::cout << "[scoped_lock_safe] Попытка обмена объекта с самим собой - пропуск" << std::endl;
        return;
    }

    std::cout << "[scoped_lock_safe] Начало обмена" << std::endl;
    std::scoped_lock lock(a.getMutex(), b.getMutex());
    a.swapData(b);
    std::cout << "[scoped_lock_safe] Обмен завершён" << std::endl;
}

void testConcurrentSwaps() {
    std::cout << "\n=== ТЕСТ В МНОГОПОТОЧНОЙ СРЕДЕ ===" << std::endl;

    Data d1(1, 100.0, "Object_A");
    Data d2(2, 200.0, "Object_B");
    Data d3(3, 300.0, "Object_C");
    Data d4(4, 400.0, "Object_D");

    std::cout << "Начальное состояние:" << std::endl;
    std::cout << "d1: "; d1.print(); std::cout << std::endl;
    std::cout << "d2: "; d2.print(); std::cout << std::endl;
    std::cout << "d3: "; d3.print(); std::cout << std::endl;
    std::cout << "d4: "; d4.print(); std::cout << std::endl;
    std::cout << std::endl;

    std::vector<std::thread> threads;

    threads.emplace_back([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        swapWithLock(d1, d2);
        });

    threads.emplace_back([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        swapWithScopedLock(d3, d4);
        });

    threads.emplace_back([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        swapWithUniqueLock(d1, d3);
        });

    threads.emplace_back([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        swapWithScopedLockSafe(d2, d4);
        });

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nКонечное состояние после многопоточных обменов:" << std::endl;
    std::cout << "d1: "; d1.print(); std::cout << std::endl;
    std::cout << "d2: "; d2.print(); std::cout << std::endl;
    std::cout << "d3: "; d3.print(); std::cout << std::endl;
    std::cout << "d4: "; d4.print(); std::cout << std::endl;
}

void testDeadlockPrevention() {
    std::cout << "\n=== ТЕСТ НА DEADLOCK ===" << std::endl;
    std::cout << "Создаём два объекта и выполняем обмены в разных порядках" << std::endl;

    Data a(1, 100.0, "A");
    Data b(2, 200.0, "B");

    std::cout << "До обмена:" << std::endl;
    std::cout << "a: "; a.print(); std::cout << std::endl;
    std::cout << "b: "; b.print(); std::cout << std::endl;

    std::cout << "\n1. swapWithLock:" << std::endl;
    swapWithLock(a, b);

    std::cout << "\n2. swapWithScopedLock:" << std::endl;
    swapWithScopedLock(a, b);

    std::cout << "\n3. swapWithUniqueLock:" << std::endl;
    swapWithUniqueLock(a, b);

    std::cout << "\nПосле обменов:" << std::endl;
    std::cout << "a: "; a.print(); std::cout << std::endl;
    std::cout << "b: "; b.print(); std::cout << std::endl;
}

void performanceTest() {
    std::cout << "\n=== ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ ===" << std::endl;

    const int ITERATIONS = 100000;

    Data d1(1, 100.0, "A");
    Data d2(2, 200.0, "B");

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        swapWithLock(d1, d2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_lock = std::chrono::duration<double>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        swapWithScopedLock(d1, d2);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_scoped = std::chrono::duration<double>(end - start).count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        swapWithUniqueLock(d1, d2);
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration_unique = std::chrono::duration<double>(end - start).count();

    std::cout << "Результаты для " << ITERATIONS << " итераций:" << std::endl;
    std::cout << "std::lock:        " << std::fixed << std::setprecision(6)
        << duration_lock << "s" << std::endl;
    std::cout << "std::scoped_lock: " << std::fixed << std::setprecision(6)
        << duration_scoped << "s" << std::endl;
    std::cout << "std::unique_lock: " << std::fixed << std::setprecision(6)
        << duration_unique << "s" << std::endl;

    std::cout << "\nОтносительная производительность (scoped_lock = 1.0):" << std::endl;
    std::cout << "std::lock:        " << duration_lock / duration_scoped << "x" << std::endl;
    std::cout << "std::scoped_lock: " << duration_scoped / duration_scoped << "x" << std::endl;
    std::cout << "std::unique_lock: " << duration_unique / duration_scoped << "x" << std::endl;
}

int main() {
    std::cout << "?????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << "?     ЗАЩИЩЁННЫЙ ОБМЕН ДАННЫМИ (SWAP) С МЬЮТЕКСАМИ           ?" << std::endl;
    std::cout << "?????????????????????????????????????????????????????????????????" << std::endl;
    std::cout << std::endl;

    std::cout << "Доступно аппаратных ядер: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << std::endl;

    std::cout << "=== БАЗОВЫЙ ОБМЕН ===" << std::endl;
    Data d1(1, 100.0, "First");
    Data d2(2, 200.0, "Second");

    std::cout << "До обмена:" << std::endl;
    std::cout << "d1: "; d1.print(); std::cout << std::endl;
    std::cout << "d2: "; d2.print(); std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Вариант 1: swapWithLock" << std::endl;
    swapWithLock(d1, d2);

    std::cout << "\nВариант 2: swapWithScopedLock" << std::endl;
    swapWithScopedLock(d1, d2);

    std::cout << "\nВариант 3: swapWithUniqueLock" << std::endl;
    swapWithUniqueLock(d1, d2);

    std::cout << "\nПосле обменов:" << std::endl;
    std::cout << "d1: "; d1.print(); std::cout << std::endl;
    std::cout << "d2: "; d2.print(); std::cout << std::endl;
    std::cout << std::endl;

    testDeadlockPrevention();
    testConcurrentSwaps();
    performanceTest();

    std::cout << "\n=== ИТОГОВЫЕ РЕКОМЕНДАЦИИ ===" << std::endl;
    std::cout << "1. std::scoped_lock - предпочтительный вариант (C++17)" << std::endl;
    std::cout << "   - Простота использования" << std::endl;
    std::cout << "   - Автоматическое предотвращение deadlock" << std::endl;
    std::cout << "   - Хорошая производительность" << std::endl;
    std::cout << std::endl;
    std::cout << "2. std::unique_lock - когда нужна гибкость" << std::endl;
    std::cout << "   - Возможность отложенной блокировки" << std::endl;
    std::cout << "   - Можно передавать владение" << std::endl;
    std::cout << "   - Условные переменные" << std::endl;
    std::cout << std::endl;
    std::cout << "3. std::lock + lock_guard - традиционный подход" << std::endl;
    std::cout << "   - Работает в C++11" << std::endl;
    std::cout << "   - Более многословный" << std::endl;
    std::cout << std::endl;

    return 0;
}