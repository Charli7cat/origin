#include "shape.h"
#include <iostream>

Shape::Shape() : sidesCount(0) {}

int Shape::getSidesCount() const {
    return sidesCount;
}

std::string Shape::getName() const {
    return "Figure";
}

bool Shape::check() const {
    return sidesCount == 0;
}

void Shape::print_info() const {
    std::cout << getName() << ":\n";
    std::cout << (check() ? "The correct one" : "Wrong") << "\n";
    std::cout << "Number of sides: " << sidesCount << "\n";
}