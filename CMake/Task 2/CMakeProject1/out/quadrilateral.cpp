#include "quadrilateral.h"
#include <iostream>

Quadrilateral::Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D)
    : Figure("The quadrilateral"), a_(a), b_(b), c_(c), d_(d), A_(A), B_(B), C_(C), D_(D) {
}

void Quadrilateral::print_info() const {
    std::cout << name_ << ":\n";
    std::cout << "Sides: a=" << a_ << " b=" << b_ << " c=" << c_ << " d=" << d_ << "\n";
    std::cout << "Ñorners: A=" << A_ << " B=" << B_ << " C=" << C_ << " D=" << D_ << "\n\n";
}