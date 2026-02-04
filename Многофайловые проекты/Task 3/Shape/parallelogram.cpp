#include "parallelogram.h"
#include <cmath>

Parallelogram::Parallelogram(double a, double b, double A, double B)
    : Quadrilateral(a, b, a, b, A, B, A, B) {
}

std::string Parallelogram::getName() const {
    return "Parallelogram";
}

bool Parallelogram::check() const {
    return Quadrilateral::check() &&
        std::abs(a - c) < 0.0001 &&
        std::abs(b - d) < 0.0001 &&
        std::abs(A - C) < 0.0001 &&
        std::abs(B - D) < 0.0001;
}