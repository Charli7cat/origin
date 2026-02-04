#ifndef PARALLELOGRAM_H
#define PARALLELOGRAM_H

#include "quadrilateral.h"

class Parallelogram : public Quadrilateral {
public:
    Parallelogram(double a, double b, double A, double B);

    std::string getName() const override;
    bool check() const override;
};

#endif
