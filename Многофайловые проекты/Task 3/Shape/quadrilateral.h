#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "shape.h"
#include <string>

class Quadrilateral : public Shape {
protected:
    double a, b, c, d;
    double A, B, C, D;

public:
    Quadrilateral(double a, double b, double c, double d,
        double A, double B, double C, double D);

    std::string getName() const override;
    bool check() const override;
    void print_info() const override;
};

#endif
