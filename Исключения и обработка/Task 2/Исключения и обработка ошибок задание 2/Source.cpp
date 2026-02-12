#include <iostream>
#include "triangle.h"
#include "quadrilateral.h"
#include "figure_exception.h"

void testFigureCreation(const std::string& figureName, Figure* figure) {
    try {
        std::cout << figureName << " (";
        figure->print_info();
        std::cout << ") generated" << std::endl;
        delete figure;
    }
    catch (const FigureException& e) {
        std::cout << "Error creating the shape. Reason: " << e.what() << std::endl;
        delete figure;
    }
}

int main() {
    testFigureCreation("A right-angled triangle",
        new RightTriangle(3, 4, 5, 30, 60));

    testFigureCreation("A right-angled triangle",
        new RightTriangle(3, 4, 5, 30, 50));

    testFigureCreation("Isosceles triangle",
        new IsoscelesTriangle(10, 8, 10, 50, 80, 50));

    testFigureCreation("Isosceles triangle",
        new IsoscelesTriangle(10, 8, 12, 50, 80, 50));

    testFigureCreation("An equilateral triangle",
        new EquilateralTriangle(6));

    testFigureCreation("An equilateral triangle",
        new EquilateralTriangle(6));

    testFigureCreation("Rectangle",
        new Rectangle(5, 8));

    testFigureCreation("Square",
        new Square(7));

    testFigureCreation("Parallelogram",
        new Parallelogram(7, 9, 45, 135));

    testFigureCreation("Rhomb",
        new Rhombus(6, 60, 120));

    testFigureCreation("Quadrilateral",
        new Quadrilateral(1, 2, 3, 4, 90, 90, 90, 80));

    testFigureCreation("Parallelogram",
        new Parallelogram(5, 7, 50, 130));

    return 0;
}