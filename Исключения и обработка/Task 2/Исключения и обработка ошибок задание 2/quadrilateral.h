#pragma once
#include "figure.h"

class Quadrilateral : public Figure {
public:
    Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D);
    bool check() const override;
};

class Rectangle : public Quadrilateral {
public:
    Rectangle(int a, int b);
};

class Square : public Rectangle {
public:
    Square(int a);
};

class Parallelogram : public Quadrilateral {
public:
    Parallelogram(int a, int b, int A, int B);
};

class Rhombus : public Parallelogram {
public:
    Rhombus(int a, int A, int B);
};
