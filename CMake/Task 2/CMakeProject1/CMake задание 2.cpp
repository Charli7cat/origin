#include <iostream>
#include <vector>
#include <memory>

#include "triangle.h"
#include "right_triangle.h"
#include "isosceles_triangle.h"
#include "equilateral_triangle.h"
#include "quadrilateral.h"
#include "rectangle.h"
#include "square.h"
#include "parallelogram.h"
#include "rhomb.h"

int main() {
    std::vector<std::unique_ptr<Figure>> figures;

    figures.push_back(std::make_unique<Triangle>(10, 20, 30, 50, 60, 70));
    figures.push_back(std::make_unique<RightTriangle>(10, 20, 30, 50, 60));
    figures.push_back(std::make_unique<IsoscelesTriangle>(10, 20, 50, 60));
    figures.push_back(std::make_unique<EquilateralTriangle>(30));
    figures.push_back(std::make_unique<Quadrilateral>(10, 20, 30, 40, 50, 60, 70, 80));
    figures.push_back(std::make_unique<Rectangle>(10, 20));
    figures.push_back(std::make_unique<Square>(20));
    figures.push_back(std::make_unique<Parallelogram>(20, 30, 30, 40));
    figures.push_back(std::make_unique<Rhomb>(30, 30, 40));

    for (const auto& figure : figures) {
        figure->print_info();
    }

    return 0;
}