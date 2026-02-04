#include "right_triangle.h"
#include <cmath>

RightTriangle::RightTriangle(double a, double b, double c, double A, double B)
    : Triangle(a, b, c, A, B, 90.0) {
}

std::string RightTriangle::getName() const {
    return "Right_triangle";
}

bool RightTriangle::check() const {
    return Triangle::check() && std::abs(C - 90.0) < 0.0001;
}