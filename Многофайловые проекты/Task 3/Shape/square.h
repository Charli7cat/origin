#ifndef SQUARE_H
#define SQUARE_H

#include "quadrilateral.h"

class Square : public Quadrilateral {
public:
    Square(double a);

    std::string getName() const override;
    bool check() const override;
};

#endif
