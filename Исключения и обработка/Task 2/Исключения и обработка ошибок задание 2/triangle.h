#pragma once
#include "figure.h"

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c, int A, int B, int C);
    bool check() const override;
};

class RightTriangle : public Triangle {
public:
    RightTriangle(int a, int b, int c, int A, int B);
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(int a, int b, int c, int A, int B, int C);
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(int a);
};
