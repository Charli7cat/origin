#pragma once
#include <stdexcept>
#include <string>

class FigureException : public std::domain_error {
public:
    FigureException(const std::string& reason) : std::domain_error(reason) {}
};
