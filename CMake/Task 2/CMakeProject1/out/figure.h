#pragma once
#include <string>

class Figure {
protected:
    std::string name_;

public:
    Figure(const std::string& name) : name_(name) {}
    virtual ~Figure() = default;

    virtual void print_info() const = 0;
    std::string get_name() const { return name_; }
};
