#include "rectangle.h"

Rectangle::Rectangle(int a, int b)
    : Quadrilateral(a, b, a, b, 90, 90, 90, 90) {
    name_ = "Rectangle";
}