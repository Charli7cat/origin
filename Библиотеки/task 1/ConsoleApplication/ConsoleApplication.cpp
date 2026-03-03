#include <iostream>
#include <string>
#include "greeter.h"

int main() {
    std::string name;

    std::cout << "Enter a name: ";
    std::getline(std::cin, name);

    Greeter greeter;
    std::cout << greeter.greet(name) << std::endl;

    return 0;
}