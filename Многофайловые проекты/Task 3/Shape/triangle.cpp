#include "triangle.h"
#include <iostream>
#include <cmath>

Triangle::Triangle(double a, double b, double c, double A, double B, double C)
    : a(a), b(b), c(c), A(A), B(B), C(C) {
    sidesCount = 3;
}

std::string Triangle::getName() const {
    return "Triangle";
}

bool Triangle::check() const {
    return std::abs(A + B + C - 180.0) < 0.0001;
}

void Triangle::print_info() const {
    Shape::print_info();
    std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
    std::cout << "The corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
}