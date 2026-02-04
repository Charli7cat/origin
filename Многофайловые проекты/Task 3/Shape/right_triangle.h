#ifndef RIGHT_TRIANGLE_H
#define RIGHT_TRIANGLE_H

#include "triangle.h"

class RightTriangle : public Triangle {
public:
    RightTriangle(double a, double b, double c, double A, double B);

    std::string getName() const override;
    bool check() const override;
};

#endif
