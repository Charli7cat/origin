#include <iostream>
#include "MyMath.h"

int main() {
    double num1, num2;
    int operation;

    std::cout << "Enter the first number: ";
    std::cin >> num1;

    std::cout << "Enter the second number: ";
    std::cin >> num2;

    std::cout << "Select an operation (1 - addition, 2 - subtraction, 3 - multiplication, 4 - division, 5 - exponentiation): ";
    std::cin >> operation;

    double result;
    bool error = false;

    try {
        switch (operation) {
        case 1:
            result = add(num1, num2);
            std::cout << num1 << " + " << num2 << " = " << result << std::endl;
            break;
        case 2:
            result = subtract(num1, num2);
            std::cout << num1 << " - " << num2 << " = " << result << std::endl;
            break;
        case 3:
            result = multiply(num1, num2);
            std::cout << num1 << " * " << num2 << " = " << result << std::endl;
            break;
        case 4:
            result = divide(num1, num2);
            std::cout << num1 << " / " << num2 << " = " << result << std::endl;
            break;
        case 5:
            result = power(num1, num2);
            std::cout << num1 << " to the extent of " << num2 << " = " << result << std::endl;
            break;
        default:
            std::cout << "Error: incorrect operation number!" << std::endl;
            error = true;
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        error = true;
    }

    if (!error && operation >= 1 && operation <= 5) {
        std::cout << "Result: " << result << std::endl;
    }

    EXIT_SUCCESS;
}