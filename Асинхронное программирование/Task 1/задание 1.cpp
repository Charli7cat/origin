#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <algorithm>
#include <random>

std::future<int> async_find_min(const std::vector<int>& arr, int start) {
    std::promise<int> promise;
    auto future = promise.get_future();

    std::thread([&arr, start, promise = std::move(promise)]() mutable {
        int min_index = start;
        for (size_t i = start + 1; i < arr.size(); ++i) {
            if (arr[i] < arr[min_index]) {
                min_index = i;
            }
        }
        promise.set_value(min_index);
        }).detach();

    return future;
}

void selection_sort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; ++i) {
        auto future = async_find_min(arr, i);

        int min_index = future.get();

        if (min_index != i) {
            std::swap(arr[i], arr[min_index]);
        }
    }
}

void print_array(const std::vector<int>& arr, const std::string& message = "") {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

std::vector<int> generate_random_array(int size, int min_val = 0, int max_val = 100) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min_val, max_val);

    std::vector<int> arr(size);
    for (int& val : arr) {
        val = dist(gen);
    }
    return arr;
}

int main() {
    std::vector<int> arr1 = { 64, 25, 12, 22, 11 };
    std::cout << "Исходный массив 1:" << std::endl;
    print_array(arr1);

    selection_sort(arr1);
    std::cout << "Отсортированный массив 1:" << std::endl;
    print_array(arr1);

    std::cout << "\n" << std::string(50, '-') << "\n\n";

    std::vector<int> arr2 = generate_random_array(10, 1, 50);
    std::cout << "Исходный массив 2 (случайный):" << std::endl;
    print_array(arr2);

    selection_sort(arr2);
    std::cout << "Отсортированный массив 2:" << std::endl;
    print_array(arr2);

    std::cout << "\n" << std::string(50, '-') << "\n\n";

    std::vector<int> arr3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::cout << "Исходный массив 3 (уже отсортирован):" << std::endl;
    print_array(arr3);

    selection_sort(arr3);
    std::cout << "Отсортированный массив 3:" << std::endl;
    print_array(arr3);

    std::cout << "\n" << std::string(50, '-') << "\n\n";

    std::vector<int> arr4 = { 5, 2, 5, 1, 5, 3, 5, 4 };
    std::cout << "Исходный массив 4 (с дубликатами):" << std::endl;
    print_array(arr4);

    selection_sort(arr4);
    std::cout << "Отсортированный массив 4:" << std::endl;
    print_array(arr4);

    return 0;
}