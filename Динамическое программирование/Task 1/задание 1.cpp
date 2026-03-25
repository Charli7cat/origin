#include <iostream>

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;
    std::cout << "Enter the number of the Fibonacci number (n >= 0): ";
    std::cin >> n;

    if (n < 0) {
        std::cout << "The number must be non-negative!" << std::endl;
        return 1;
    }

    int result = fibonacci(n);
    std::cout << "F(" << n << ") = " << result << std::endl;

    return 0;
}