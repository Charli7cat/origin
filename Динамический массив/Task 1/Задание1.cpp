#include <iostream>

void print_dynamic_array(int* arr, int logical_size, int actual_size) {
    for (int i = 0; i < actual_size; i++) {
        if (i < logical_size) {
            std::cout << arr[i];
        }
        else {
            std::cout << '_';
        }

        if (i < actual_size - 1) {
            std::cout << ' ';
        }
    }
    std::cout << std::endl;
}

int main() {
    int actual_size, logical_size;

    std::cout << "Enter the actual size of the array: ";
    std::cin >> actual_size;

    std::cout << "Enter the logical size of the array: ";
    std::cin >> logical_size;

    if (logical_size > actual_size) {
        std::cout << "Error! The logical size of an array cannot exceed the actual size!" << std::endl;
        return 1;
    }

    int* arr = new int[actual_size];

    for (int i = 0; i < logical_size; i++) {
        std::cout << "Enter arr[" << i << "]: ";
        std::cin >> arr[i];
    }

    std::cout << "Dynamic array: ";
    print_dynamic_array(arr, logical_size, actual_size);

    delete[] arr;

    return 0;
}