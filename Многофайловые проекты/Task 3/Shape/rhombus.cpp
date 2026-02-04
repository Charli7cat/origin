#include "rhombus.h"
#include <cmath>

Rhombus::Rhombus(double a, double A, double B)
    : Quadrilateral(a, a, a, a, A, B, A, B) {
}

std::string Rhombus::getName() const {
    return "Rhombus";
}

bool Rhombus::check() const {
    return Quadrilateral::check() &&
        std::abs(a - b) < 0.0001 &&
        std::abs(a - c) < 0.0001 &&
        std::abs(a - d) < 0.0001 &&
        std::abs(A - C) < 0.0001 &&
        std::abs(B - D) < 0.0001;
}