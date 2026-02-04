#include "MyMath.h"
#include <cmath>
#include <stdexcept>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        throw std::runtime_error("Error: division by zero!");
    }
    return a / b;
}

double power(double a, double b) {
    return std::pow(a, b);
}