#include "isosceles_triangle.h"
#include <cmath>

IsoscelesTriangle::IsoscelesTriangle(double a, double b, double A, double B)
    : Triangle(a, b, a, A, B, A) {
}

std::string IsoscelesTriangle::getName() const {
    return "isosceles_triangle";
}

bool IsoscelesTriangle::check() const {
    return Triangle::check() &&
        std::abs(a - c) < 0.0001 &&
        std::abs(A - C) < 0.0001;
}