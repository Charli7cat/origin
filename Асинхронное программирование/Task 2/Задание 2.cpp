#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <thread>
#include <future>
#include <chrono>
#include <random>
#include <numeric>

template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func&& func) {
    auto distance = std::distance(first, last);

    const size_t MIN_BLOCK_SIZE = 25;

    if (distance <= MIN_BLOCK_SIZE) {
        std::for_each(first, last, std::forward<Func>(func));
        return;
    }

    auto mid = first;
    std::advance(mid, distance / 2);

    auto future = std::async(std::launch::async,
        [mid, last, &func]() {
            parallel_for_each(mid, last, std::forward<Func>(func));
        });

    parallel_for_each(first, mid, std::forward<Func>(func));

    future.wait();
}

template<typename Iterator, typename Func>
void parallel_for_each_advanced(Iterator first, Iterator last, Func&& func,
    int max_depth = 4) {
    auto distance = std::distance(first, last);
    const size_t MIN_BLOCK_SIZE = 100;

    if (max_depth <= 0 || distance <= MIN_BLOCK_SIZE) {
        std::for_each(first, last, std::forward<Func>(func));
        return;
    }

    auto mid = first;
    std::advance(mid, distance / 2);

    auto future = std::async(std::launch::async,
        [mid, last, &func, max_depth]() {
            parallel_for_each_advanced(mid, last,
                std::forward<Func>(func),
                max_depth - 1);
        });

    parallel_for_each_advanced(first, mid, std::forward<Func>(func), max_depth - 1);

    future.wait();
}

template<typename Iterator, typename Func>
std::future<void> parallel_for_each_async(Iterator first, Iterator last, Func&& func) {
    return std::async(std::launch::async,
        [first, last, func = std::forward<Func>(func)]() {
            parallel_for_each(first, last, func);
        });
}

template<typename Container>
void print_container(const Container& c, const std::string& message = "") {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    for (const auto& val : c) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int random_int(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

int main() {
    std::cout << "=== Демонстрация параллельного for_each ===\n\n";

    std::cout << "Пример 1: Вектор целых чисел (умножение на 2)\n";
    std::vector<int> vec(1000);
    std::generate(vec.begin(), vec.end(), []() { return random_int(1, 100); });

    std::cout << "Первые 10 элементов до обработки: ";
    for (int i = 0; i < 10; ++i) std::cout << vec[i] << " ";
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    parallel_for_each(vec.begin(), vec.end(), [](int& x) {
        x *= 2;
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        });

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Первые 10 элементов после обработки: ";
    for (int i = 0; i < 10; ++i) std::cout << vec[i] << " ";
    std::cout << "\nВремя выполнения: " << duration.count() << " мс\n\n";

    std::cout << "Пример 2: Список строк (добавление суффикса)\n";
    std::list<std::string> str_list = { "apple", "banana", "cherry", "date", "elderberry",
                                       "fig", "grape", "honeydew", "kiwi", "lemon" };

    std::cout << "Строки до обработки: ";
    for (const auto& s : str_list) std::cout << s << " ";
    std::cout << std::endl;

    parallel_for_each(str_list.begin(), str_list.end(), [](std::string& s) {
        s += "_processed";
        });

    std::cout << "Строки после обработки: ";
    for (const auto& s : str_list) std::cout << s << " ";
    std::cout << "\n\n";

    std::cout << "Пример 3: Сравнение производительности\n";
    const int SIZE = 10000;
    std::vector<int> vec_parallel(SIZE);
    std::vector<int> vec_sequential(SIZE);

    std::generate(vec_parallel.begin(), vec_parallel.end(),
        []() { return random_int(1, 1000); });
    vec_sequential = vec_parallel;

    start = std::chrono::high_resolution_clock::now();
    parallel_for_each(vec_parallel.begin(), vec_parallel.end(),
        [](int& x) {
            x = x * x;
            std::this_thread::sleep_for(std::chrono::microseconds(5));
        });
    end = std::chrono::high_resolution_clock::now();
    auto parallel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    std::for_each(vec_sequential.begin(), vec_sequential.end(),
        [](int& x) {
            x = x * x;
            std::this_thread::sleep_for(std::chrono::microseconds(5));
        });
    end = std::chrono::high_resolution_clock::now();
    auto sequential_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Размер контейнера: " << SIZE << " элементов\n";
    std::cout << "Параллельное время: " << parallel_time.count() << " мс\n";
    std::cout << "Последовательное время: " << sequential_time.count() << " мс\n";
    std::cout << "Ускорение: " << (double)sequential_time.count() / parallel_time.count() << "x\n\n";

    std::cout << "Пример 4: Асинхронный запуск с future\n";
    std::vector<int> data(100);
    std::generate(data.begin(), data.end(), []() { return random_int(1, 50); });

    std::cout << "Сумма элементов до обработки: "
        << std::accumulate(data.begin(), data.end(), 0) << std::endl;

    auto future = parallel_for_each_async(data.begin(), data.end(),
        [](int& x) { x += 10; });

    std::cout << "Ожидание завершения параллельной обработки...\n";
    future.wait();

    std::cout << "Сумма элементов после обработки: "
        << std::accumulate(data.begin(), data.end(), 0) << "\n\n";

    std::cout << "Пример 5: Обработка большого массива\n";
    const int LARGE_SIZE = 100000;
    std::vector<double> large_vec(LARGE_SIZE);
    std::generate(large_vec.begin(), large_vec.end(),
        []() { return random_int(1, 100) / 10.0; });

    start = std::chrono::high_resolution_clock::now();

    parallel_for_each(large_vec.begin(), large_vec.end(),
        [](double& x) {
            x = std::sqrt(x);
        });

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Обработано " << LARGE_SIZE << " элементов за "
        << duration.count() << " мс\n";
    std::cout << "Первые 10 результатов: ";
    for (int i = 0; i < 10 && i < large_vec.size(); ++i) {
        std::cout << large_vec[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}