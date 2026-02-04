#include <iostream>
#include <string>

class Shape {
protected:
    int sidesCount;

public:
    Shape() : sidesCount(0) {}

    int getSidesCount() const {
        return sidesCount;
    }

    virtual void printInfo() const = 0;
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

    void printInfo() const override {
        std::cout << "Triangle :\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
    }

    double getSideA() const { return a; }
    double getSideB() const { return b; }
    double getSideC() const { return c; }

    double getAngleA() const { return A; }
    double getAngleB() const { return B; }
    double getAngleC() const { return C; }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(double a, double b, double c, double A, double B)
        : Triangle(a, b, c, A, B, 90.0) {
    }

    void printInfo() const override {
        std::cout << "A right-angled triangle:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(double a, double b, double A, double B)
        : Triangle(a, b, a, A, B, A) {
    }

    void printInfo() const override {
        std::cout << "Isosceles triangle:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
    }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(double a)
        : Triangle(a, a, a, 60.0, 60.0, 60.0) {
    }

    void printInfo() const override {
        std::cout << "An equilateral triangle:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << "\n\n";
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

    void printInfo() const override {
        std::cout << "Quadrilateral:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }

    double getSideA() const { return a; }
    double getSideB() const { return b; }
    double getSideC() const { return c; }
    double getSideD() const { return d; }

    double getAngleA() const { return A; }
    double getAngleB() const { return B; }
    double getAngleC() const { return C; }
    double getAngleD() const { return D; }
};

class Rectangle : public Quadrilateral {
public:
    Rectangle(double a, double b)
        : Quadrilateral(a, b, a, b, 90.0, 90.0, 90.0, 90.0) {
    }

    void printInfo() const override {
        std::cout << "Rectangle:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }
};

class Square : public Quadrilateral {
public:
    Square(double a)
        : Quadrilateral(a, a, a, a, 90.0, 90.0, 90.0, 90.0) {
    }

    void printInfo() const override {
        std::cout << "Square:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }
};

class Parallelogram : public Quadrilateral {
public:
    Parallelogram(double a, double b, double A, double B)
        : Quadrilateral(a, b, a, b, A, B, A, B) {
    }

    void printInfo() const override {
        std::cout << "Parallelogram:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }
};

class Rhombus : public Quadrilateral {
public:
    Rhombus(double a, double A, double B)
        : Quadrilateral(a, a, a, a, A, B, A, B) {
    }

    void printInfo() const override {
        std::cout << "Rhomb:\n";
        std::cout << "Sides: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n";
        std::cout << "Corners: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n\n";
    }
};

void print_info(const Shape* shape) {
    shape->printInfo();
}

int main() {
    Triangle triangle(10, 20, 30, 50, 60, 70);
    RightTriangle rightTriangle(10, 20, 30, 50, 60);
    IsoscelesTriangle isoscelesTriangle(10, 20, 50, 60);
    EquilateralTriangle equilateralTriangle(30);

    Quadrilateral quadrilateral(10, 20, 30, 40, 50, 60, 70, 80);
    Rectangle rectangle(10, 20);
    Square square(20);
    Parallelogram parallelogram(20, 30, 30, 40);
    Rhombus rhombus(30, 30, 40);

    print_info(&triangle);
    print_info(&rightTriangle);
    print_info(&isoscelesTriangle);
    print_info(&equilateralTriangle);
    print_info(&quadrilateral);
    print_info(&rectangle);
    print_info(&square);
    print_info(&parallelogram);
    print_info(&rhombus);

   EXIT_SUCCESS;
}