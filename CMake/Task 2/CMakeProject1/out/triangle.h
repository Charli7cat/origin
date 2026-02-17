#pragma once
#include "figure.h"

class Triangle : public Figure {
protected:
    int a_, b_, c_;
    int A_, B_, C_;

public:
    Triangle(int a, int b, int c, int A, int B, int C);
    virtual void print_info() const override;
};
