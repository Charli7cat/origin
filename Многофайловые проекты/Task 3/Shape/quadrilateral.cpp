#include "quadrilateral.h"
#include <iostream>
#include <cmath>

Quadrilateral::Quadrilateral(double a, double b, double c, double d,
    double A, double B, double C, double D)
    : a(a), b(b), c(c), d(d), A(A), B(B), C(C), D(D) {
    sidesCount = 4;
}

std::string Quadrilateral::getName() const {
    return "Quadrilateral";
}

bool Quadrilateral::check() const {
    return std::abs(A + B + C + D - 360.0) < 0.0001;
}

void Quadrilateral::print_info() const {
    Shape::print_info();
    std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
    std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
}