#include "figure.h"
#include <iostream>

Figure::Figure() : sides_count(0) {
    name = "figure";
}

bool Figure::check() const {
    return sides_count == 0;
}

void Figure::print_info() const {
    std::cout << name << " (sides: ";
    for (int i = 0; i < sides.size(); ++i) {
        std::cout << sides[i];
        if (i < sides.size() - 1) std::cout << ", ";
    }

    std::cout << "; Ñorners: ";
    for (int i = 0; i < angles.size(); ++i) {
        std::cout << angles[i];
        if (i < angles.size() - 1) std::cout << ", ";
    }
    std::cout << ")";
}

const std::string& Figure::get_name() const {
    return name;
}