#include <iostream>
#include <string>

unsigned int simple_string_hash(const std::string& str) {
    unsigned int hash = 0;
    for (char c : str) {
        hash += static_cast<unsigned int>(c);
    }
    return hash;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "Enter the line: ";
        std::getline(std::cin, input);

        unsigned int hash = simple_string_hash(input);
        std::cout << "Naive hash of a string " << input << " = " << hash << std::endl;

        if (input == "exit") {
            break;
        }
    }

    return 0;
}