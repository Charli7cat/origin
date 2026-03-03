#include <iostream>
#include <string>
#include "leaver.h"

int main() {
    std::string name;

    std::cout << "Enter a name: ";
    std::getline(std::cin, name);

    Leaver leaver;
    std::cout << leaver.leave(name) << std::endl;

    return 0;
}