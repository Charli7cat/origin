#include "quadrilateral.h"
#include "figure_exception.h"
#include <iostream>

Quadrilateral::Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D) : Figure() {
    sides = { a, b, c, d };
    angles = { A, B, C, D };
    sides_count = 4;
    name = "Quadrilateral";

    if (!check()) {
        throw FigureException("the sum of the angles is not equal to 360");
    }
}

bool Quadrilateral::check() const {
    if (sides.size() != 4 || angles.size() != 4) return false;
    if (sides_count != 4) return false;

    int sum_angles = 0;
    for (int angle : angles) {
        sum_angles += angle;
    }
    return sum_angles == 360;
}

Rectangle::Rectangle(int a, int b)
    : Quadrilateral(a, b, a, b, 90, 90, 90, 90) {
    name = "Rectangle";

    if (sides[0] != sides[2] || sides[1] != sides[3]) {
        throw FigureException("the sides a,c and b,d must be equal to each other");
    }
    for (int angle : angles) {
        if (angle != 90) {
            throw FigureException("all angles must be 90 degrees");
        }
    }
}

Square::Square(int a) : Rectangle(a, a) {
    name = "Square";

    if (sides[0] != sides[1] || sides[1] != sides[2] || sides[2] != sides[3]) {
        throw FigureException("All parties must be equal");
    }
}

Parallelogram::Parallelogram(int a, int b, int A, int B)
    : Quadrilateral(a, b, a, b, A, B, A, B) {
    name = "Parallelogram";

    if (sides[0] != sides[2] || sides[1] != sides[3]) {
        throw FigureException("the sides a,c and b,d must be equal to each other");
    }
    if (angles[0] != angles[2] || angles[1] != angles[3]) {
        throw FigureException("angles A,C and B,D must be equal in pairs");
    }
}

Rhombus::Rhombus(int a, int A, int B) : Parallelogram(a, a, A, B) {
    name = "Rhomb";

    if (sides[0] != sides[1] || sides[1] != sides[2] || sides[2] != sides[3]) {
        throw FigureException("All parties must be equal");
    }
}