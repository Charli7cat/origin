#ifndef EQUILATERAL_TRIANGLE_H
#define EQUILATERAL_TRIANGLE_H

#include "triangle.h"

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(double a);

    std::string getName() const override;
    bool check() const override;
};

#endif
