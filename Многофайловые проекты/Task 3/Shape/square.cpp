#include "square.h"
#include <cmath>

Square::Square(double a)
    : Quadrilateral(a, a, a, a, 90.0, 90.0, 90.0, 90.0) {
}

std::string Square::getName() const {
    return "Square";
}

bool Square::check() const {
    return Quadrilateral::check() &&
        std::abs(a - b) < 0.0001 &&
        std::abs(a - c) < 0.0001 &&
        std::abs(a - d) < 0.0001 &&
        std::abs(A - 90.0) < 0.0001 &&
        std::abs(B - 90.0) < 0.0001 &&
        std::abs(C - 90.0) < 0.0001 &&
        std::abs(D - 90.0) < 0.0001;
}