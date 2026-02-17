#include "rhomb.h"

Rhomb::Rhomb(int a, int A, int B)
    : Quadrilateral(a, a, a, a, A, B, A, B) {
    name_ = "Rhomb";
}