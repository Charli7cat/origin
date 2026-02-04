#include <iostream>
#include <cmath>

class Shape {
protected:
    int sidesCount;

public:
    Shape() : sidesCount(0) {}

    virtual std::string getName() const {
        return "Figure";
    }

    virtual bool check() const {
        return sidesCount == 0;
    }

    virtual void print_info() const {
        std::cout << getName() << ":\n";
        std::cout << (check() ? "The correct one" : "Wrong") << "\n";
        std::cout << "Number of sides: " << sidesCount << "\n";
    }

    virtual ~Shape() = default;
};

class Triangle : public Shape {
protected:
    double a, b, c;
    double A, B, C;

public:
    Triangle(double a, double b, double c, double A, double B, double C)
        : a(a), b(b), c(c), A(A), B(B), C(C) {
        sidesCount = 3;
    }

    std::string getName() const override {
        return "Triangle";
    }

    bool check() const override {
        return std::abs(A + B + C - 180.0) < 0.0001;
    }

    void print_info() const override {
        Shape::print_info();
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
    }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(double a, double b, double c, double A, double B)
        : Triangle(a, b, c, A, B, 90.0) {
    }

    std::string getName() const override {
        return "A right-angled triangle";
    }

    bool check() const override {
        return Triangle::check() && std::abs(C - 90.0) < 0.0001;
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(double a, double b, double A, double B)
        : Triangle(a, b, a, A, B, A) {
    }

    std::string getName() const override {
        return "Isosceles triangle";
    }

    bool check() const override {
        return Triangle::check() &&
            std::abs(a - c) < 0.0001 &&
            std::abs(A - C) < 0.0001;
    }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(double a)
        : Triangle(a, a, a, 60.0, 60.0, 60.0) {
    }

    std::string getName() const override {
        return "An equilateral triangle";
    }

    bool check() const override {
        return Triangle::check() &&
            std::abs(a - b) < 0.0001 &&
            std::abs(a - c) < 0.0001 &&
            std::abs(A - 60.0) < 0.0001 &&
            std::abs(B - 60.0) < 0.0001 &&
            std::abs(C - 60.0) < 0.0001;
    }
};

class Quadrilateral : public Shape {
protected:
    double a, b, c, d;
    double A, B, C, D;

public:
    Quadrilateral(double a, double b, double c, double d,
        double A, double B, double C, double D)
        : a(a), b(b), c(c), d(d), A(A), B(B), C(C), D(D) {
        sidesCount = 4;
    }

    std::string getName() const override {
        return "Quadrilateral";
    }

    bool check() const override {
        return std::abs(A + B + C + D - 360.0) < 0.0001;
    }

    void print_info() const override {
        Shape::print_info();
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }
};

class Rectangle : public Quadrilateral {
public:
    Rectangle(double a, double b)
        : Quadrilateral(a, b, a, b, 90.0, 90.0, 90.0, 90.0) {
    }

    std::string getName() const override {
        return "Rectangle";
    }

    bool check() const override {
        return Quadrilateral::check() &&
            std::abs(a - c) < 0.0001 &&
            std::abs(b - d) < 0.0001 &&
            std::abs(A - 90.0) < 0.0001 &&
            std::abs(B - 90.0) < 0.0001 &&
            std::abs(C - 90.0) < 0.0001 &&
            std::abs(D - 90.0) < 0.0001;
    }
};

class Square : public Quadrilateral {
public:
    Square(double a)
        : Quadrilateral(a, a, a, a, 90.0, 90.0, 90.0, 90.0) {
    }

    std::string getName() const override {
        return "Square";
    }

    bool check() const override {
        return Quadrilateral::check() &&
            std::abs(a - b) < 0.0001 &&
            std::abs(a - c) < 0.0001 &&
            std::abs(a - d) < 0.0001 &&
            std::abs(A - 90.0) < 0.0001 &&
            std::abs(B - 90.0) < 0.0001 &&
            std::abs(C - 90.0) < 0.0001 &&
            std::abs(D - 90.0) < 0.0001;
    }
};

class Parallelogram : public Quadrilateral {
public:
    Parallelogram(double a, double b, double A, double B)
        : Quadrilateral(a, b, a, b, A, B, A, B) {
    }

    std::string getName() const override {
        return "Parallelogram";
    }

    bool check() const override {
        return Quadrilateral::check() &&
            std::abs(a - c) < 0.0001 &&
            std::abs(b - d) < 0.0001 &&
            std::abs(A - C) < 0.0001 &&
            std::abs(B - D) < 0.0001;
    }
};

class Rhombus : public Quadrilateral {
public:
    Rhombus(double a, double A, double B)
        : Quadrilateral(a, a, a, a, A, B, A, B) {
    }

    std::string getName() const override {
        return "Rhomb";
    }

    bool check() const override {
        return Quadrilateral::check() &&
            std::abs(a - b) < 0.0001 &&
            std::abs(a - c) < 0.0001 &&
            std::abs(a - d) < 0.0001 &&
            std::abs(A - C) < 0.0001 &&
            std::abs(B - D) < 0.0001;
    }
};

int main() {
    Shape* shapes[11];

    shapes[0] = new Shape();
    shapes[1] = new Triangle(10, 20, 30, 50, 60, 70);
    shapes[2] = new RightTriangle(10, 20, 30, 50, 60);
    shapes[3] = new RightTriangle(10, 20, 30, 50, 40);
    shapes[4] = new IsoscelesTriangle(10, 20, 50, 60);
    shapes[5] = new EquilateralTriangle(30);
    shapes[6] = new Quadrilateral(10, 20, 30, 40, 50, 60, 70, 80);
    shapes[7] = new Rectangle(10, 20);
    shapes[8] = new Square(20);
    shapes[9] = new Parallelogram(20, 30, 30, 40);
    shapes[10] = new Rhombus(30, 30, 40);

    for (int i = 0; i < 11; i++) {
        shapes[i]->print_info();
    }

    for (int i = 0; i < 11; i++) {
        delete shapes[i];
    }

    EXIT_SUCCESS;
}