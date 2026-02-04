#include "rectangle.h"
#include <cmath>

Rectangle::Rectangle(double a, double b)
    : Quadrilateral(a, b, a, b, 90.0, 90.0, 90.0, 90.0) {
}

std::string Rectangle::getName() const {
    return "Rectangle";
}

bool Rectangle::check() const {
    return Quadrilateral::check() &&
        std::abs(a - c) < 0.0001 &&
        std::abs(b - d) < 0.0001 &&
        std::abs(A - 90.0) < 0.0001 &&
        std::abs(B - 90.0) < 0.0001 &&
        std::abs(C - 90.0) < 0.0001 &&
        std::abs(D - 90.0) < 0.0001;
}