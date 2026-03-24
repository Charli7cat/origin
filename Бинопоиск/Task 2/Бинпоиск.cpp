#include <iostream>

int countGreater(int* arr, int size, int target) {
    int left = 0;
    int right = size;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] <= target) {
            left = mid + 1;
        }
        else {
            right = mid;
        }
    }

    return size - left;
}

int main() {
    int arr[] = { 14, 16, 19, 32, 32, 32, 56, 69, 72 };
    int size = sizeof(arr) / sizeof(arr[0]);

    int target;
    std::cout << "Enter a reference point: ";
    std::cin >> target;

    int result = countGreater(arr, size, target);
    std::cout << "The number of elements in the array is greater than " << target << ": " << result << std::endl;

    return 0;
}

