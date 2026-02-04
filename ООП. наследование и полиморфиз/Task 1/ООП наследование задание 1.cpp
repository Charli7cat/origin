#include <iostream>

class Shape {
protected:
	int sidesCount;

public:
	Shape() : sidesCount(0) {}
	int getSidesCount() const {
		return sidesCount;
	}

	virtual ~Shape() = default;
};

class Triangle : public Shape {
public:
	Triangle() {
		sidesCount = 3;
	}
};

class Quadrilateral : public Shape {
public:
	Quadrilateral() {
		sidesCount = 4;
	}
};

int main() {
	Shape shape;
	Triangle triangle;
	Quadrilateral quad;

	std::cout << "Number of sides: " << std::endl;
	std::cout << "Figure: " << shape.getSidesCount() << std::endl;
	std::cout << "Triangle: " << triangle.getSidesCount() << std::endl;
	std::cout << "Quadrangle: " << quad.getSidesCount() << std::endl;


	EXIT_SUCCESS;
}