#include <iostream>
#include <vector>

int fibonacciMemo(int n, std::vector<int>& memo) {
    if (memo[n] != -1) {
        return memo[n];
    }

    memo[n] = fibonacciMemo(n - 1, memo) + fibonacciMemo(n - 2, memo);
    return memo[n];
}

int fibonacci(int n) {
    if (n < 0) {
        return -1;
    }

    std::vector<int> memo(n + 1, -1);

    if (n >= 0) memo[0] = 0;
    if (n >= 1) memo[1] = 1;

    return fibonacciMemo(n, memo);
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