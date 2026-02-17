#include "equilateral_triangle.h"

EquilateralTriangle::EquilateralTriangle(int a)
    : Triangle(a, a, a, 60, 60, 60) {
    name_ = "An equilateral triangle";
}