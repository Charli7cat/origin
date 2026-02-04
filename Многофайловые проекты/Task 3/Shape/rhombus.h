#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "quadrilateral.h"

class Rhombus : public Quadrilateral {
public:
    Rhombus(double a, double A, double B);

    std::string getName() const override;
    bool check() const override;
};

#endif
