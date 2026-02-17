#include "triangle.h"
#include <iostream>

Triangle::Triangle(int a, int b, int c, int A, int B, int C)
    : Figure("Triangle"), a_(a), b_(b), c_(c), A_(A), B_(B), C_(C) {
}

void Triangle::print_info() const {
    std::cout << name_ << ":\n";
    std::cout << "Sides: a=" << a_ << " b=" << b_ << " c=" << c_ << "\n";
    std::cout << "Corners: A=" << A_ << " B=" << B_ << " C=" << C_ << "\n\n";
}