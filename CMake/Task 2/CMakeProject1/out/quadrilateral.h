#pragma once
#include "figure.h"

class Quadrilateral : public Figure {
protected:
    int a_, b_, c_, d_;
    int A_, B_, C_, D_;

public:
    Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D);
    virtual void print_info() const override;
};
