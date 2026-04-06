#include <iostream>
#include <vector>
#include <cmath>

int get_level(int index) {
    int level = 0;
    while (pow(2, level + 1) - 1 <= index) {
        level++;
    }
    return level;
}

void print_pyramid(const std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }

    int n = arr.size();

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            std::cout << 0 << " root " << arr[i] << std::endl;
        }
        else {
            int parent_index = (i - 1) / 2;
            int parent_value = arr[parent_index];

            if (i % 2 == 1) {
                std::cout << get_level(i) << " left(" << parent_value << ") " << arr[i] << std::endl;
            }
            else {
                std::cout << get_level(i) << " right(" << parent_value << ") " << arr[i] << std::endl;
            }
        }
    }
}

void test_pyramid(const std::vector<int>& arr, const std::string& test_name = "") {
    if (!test_name.empty()) {
        std::cout << "\n=== " << test_name << " ===" << std::endl;
    }

    std::cout << "The original array: ";
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    std::cout << "Pyramid:" << std::endl;
    print_pyramid(arr);
}

int main() {
    test_pyramid({ 1, 3, 6, 5, 9, 8 }, "Example 1");
    test_pyramid({ 94, 67, 18, 44, 55, 12, 6, 42 }, "Example 2");
    test_pyramid({ 16, 11, 9, 10, 5, 6, 8, 1, 2, 4 }, "Example 3");
    test_pyramid({ 42 }, "Test: one element");
    test_pyramid({}, "Test: an empty pyramid");

    return 0;
}