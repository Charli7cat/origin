#ifndef SHAPE_H
#define SHAPE_H

#include <string>

class Shape {
protected:
    int sidesCount;

public:
    Shape();
    virtual ~Shape() = default;

    int getSidesCount() const;
    virtual std::string getName() const;
    virtual bool check() const;
    virtual void print_info() const;
};

#endif