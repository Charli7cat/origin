#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include <string>

class Triangle : public Shape {
protected:
    double a, b, c;
    double A, B, C;

public:
    Triangle(double a, double b, double c, double A, double B, double C);

    std::string getName() const override;
    bool check() const override;
    void print_info() const override;
};

#endif
