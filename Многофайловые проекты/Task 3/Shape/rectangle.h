#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "quadrilateral.h"

class Rectangle : public Quadrilateral {
public:
    Rectangle(double a, double b);

    std::string getName() const override;
    bool check() const override;
};

#endif
