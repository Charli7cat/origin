#include <iostream>
#include <string>

int main() {
    std::string name;

    std::cout << "Enter a name: ";
    std::getline(std::cin, name);

    std::cout << "Hello, " << name << "!" << std::endl;

    return 0;
}