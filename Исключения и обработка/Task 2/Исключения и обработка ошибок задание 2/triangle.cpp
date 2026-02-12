#include "triangle.h"
#include "figure_exception.h"
#include <iostream>

Triangle::Triangle(int a, int b, int c, int A, int B, int C) : Figure() {
    sides = { a, b, c };
    angles = { A, B, C };
    sides_count = 3;
    name = "Triangle";

    if (!check()) {
        throw FigureException("the sum of the angles is not equal to 180");
    }
}

bool Triangle::check() const {
    if (sides.size() != 3 || angles.size() != 3) return false;
    if (sides_count != 3) return false;

    int sum_angles = 0;
    for (int angle : angles) {
        sum_angles += angle;
    }
    return sum_angles == 180;
}

RightTriangle::RightTriangle(int a, int b, int c, int A, int B)
    : Triangle(a, b, c, A, B, 90) {
    name = "A right-angled triangle";

    if (angles[2] != 90) {
        throw FigureException("angle C is not equal to 90");
    }
}

IsoscelesTriangle::IsoscelesTriangle(int a, int b, int c, int A, int B, int C)
    : Triangle(a, b, c, A, B, C) {
    name = "Isosceles triangle";

    if (sides[0] != sides[2]) {
        throw FigureException("sides a and c are not equal");
    }
    if (angles[0] != angles[2]) {
        throw FigureException("angles A and C are not equal");
    }
}

EquilateralTriangle::EquilateralTriangle(int a)
    : Triangle(a, a, a, 60, 60, 60) {
    name = "An equilateral triangle";

    if (sides[0] != sides[1] || sides[1] != sides[2]) {
        throw FigureException("The parties are not equal");
    }
    if (angles[0] != 60 || angles[1] != 60 || angles[2] != 60) {
        throw FigureException("The angles are not equal to 60");
    }
}