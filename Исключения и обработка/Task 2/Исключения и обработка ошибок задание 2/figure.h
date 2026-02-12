#pragma once
#include <string>
#include <vector>

class Figure {
protected:
    std::string name;
    std::vector<int> sides;
    std::vector<int> angles;
    int sides_count;

public:
    Figure();
    virtual bool check() const;
    virtual void print_info() const;
    const std::string& get_name() const;
};
