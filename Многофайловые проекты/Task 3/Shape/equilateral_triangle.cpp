#include "equilateral_triangle.h"
#include <cmath>

EquilateralTriangle::EquilateralTriangle(double a)
    : Triangle(a, a, a, 60.0, 60.0, 60.0) {
}

std::string EquilateralTriangle::getName() const {
    return "quilateral_triangle";
}

bool EquilateralTriangle::check() const {
    return Triangle::check() &&
        std::abs(a - b) < 0.0001 &&
        std::abs(a - c) < 0.0001 &&
        std::abs(A - 60.0) < 0.0001 &&
        std::abs(B - 60.0) < 0.0001 &&
        std::abs(C - 60.0) < 0.0001;
}