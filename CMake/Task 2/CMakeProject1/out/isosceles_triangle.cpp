#include "isosceles_triangle.h"

IsoscelesTriangle::IsoscelesTriangle(int a, int b, int A, int B)
    : Triangle(a, b, a, A, B, A) {
    name_ = "Isosceles triangle";
}