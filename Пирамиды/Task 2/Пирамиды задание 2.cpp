#include <iostream>
#include <vector>
#include <string>
#include <cmath>

int get_level(int index) {
    int level = 0;
    while (pow(2, level + 1) - 1 <= index) {
        level++;
    }
    return level;
}

void print_element(const std::vector<int>& arr, int index) {
    if (index == 0) {
        std::cout << get_level(index) << " root " << arr[index];
    }
    else {
        int parent_index = (index - 1) / 2;
        int parent_value = arr[parent_index];

        if (index % 2 == 1) {
            std::cout << get_level(index) << " left(" << parent_value << ") " << arr[index];
        }
        else {
            std::cout << get_level(index) << " right(" << parent_value << ") " << arr[index];
        }
    }
}

void print_pyramid(const std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }

    int n = arr.size();

    for (int i = 0; i < n; i++) {
        print_element(arr, i);
        std::cout << std::endl;
    }
}

bool has_left_child(int index, int size) {
    int left_child_index = 2 * index + 1;
    return left_child_index < size;
}

bool has_right_child(int index, int size) {
    int right_child_index = 2 * index + 2;
    return right_child_index < size;
}

bool has_parent(int index) {
    return index > 0;
}

int get_parent_index(int index) {
    return (index - 1) / 2;
}

int get_left_child_index(int index) {
    return 2 * index + 1;
}

int get_right_child_index(int index) {
    return 2 * index + 2;
}

void travel_pyramid(const std::vector<int>& arr) {
    if (arr.empty()) {
        std::cout << "The pyramid is empty!" << std::endl;
        return;
    }

    int current_index = 0;
    int size = arr.size();
    std::string command;

    while (true) {
        std::cout << "\nYou are here: ";
        print_element(arr, current_index);
        std::cout << std::endl;

        std::cout << "Enter the command: ";
        std::cin >> command;

        if (command == "exit") {
            break;
        }
        else if (command == "up") {
            if (has_parent(current_index)) {
                current_index = get_parent_index(current_index);
                std::cout << "Ok" << std::endl;
            }
            else {
                std::cout << "Mistake! A parent is missing" << std::endl;
            }
        }
        else if (command == "left") {
            if (has_left_child(current_index, size)) {
                current_index = get_left_child_index(current_index);
                std::cout << "Ok" << std::endl;
            }
            else {
                std::cout << "Mistake! The left descendant is missing" << std::endl;
            }
        }
        else if (command == "right") {
            if (has_right_child(current_index, size)) {
                current_index = get_right_child_index(current_index);
                std::cout << "Ok" << std::endl;
            }
            else {
                std::cout << "Mistake! The right descendant is missing" << std::endl;
            }
        }
        else {
            std::cout << "Error! Unknown command. Available commands: up, left, right, exit" << std::endl;
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

    travel_pyramid(arr);
}

int main() {
    std::vector<int> pyramid1 = { 1, 3, 6, 5, 9, 8 };
    test_pyramid(pyramid1, "Pyramid 1");

    return 0;
}