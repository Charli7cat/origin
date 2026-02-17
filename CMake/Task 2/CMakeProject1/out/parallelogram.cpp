#include "parallelogram.h"

Parallelogram::Parallelogram(int a, int b, int A, int B)
    : Quadrilateral(a, b, a, b, A, B, A, B) {
    name_ = "Parallelogram";
}