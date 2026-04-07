#include <iostream>
#include <string>
#include <cstdint>


uint64_t real_string_hash(const std::string& str, uint64_t p, uint64_t n) {
    uint64_t hash = 0;
    uint64_t power = 1;

    for (size_t i = 0; i < str.length(); ++i) {
        hash += static_cast<uint64_t>(str[i]) * power;

        power *= p;

    }

    return hash % n;
}

int main() {
    uint64_t p, n;
    std::string input;

    std::cout << "Enter p: ";
    std::cin >> p;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cin.ignore();

    while (true) {
        std::cout << "Enter the line: ";
        std::getline(std::cin, input);

        uint64_t hash = real_string_hash(input, p, n);
        std::cout << "Hash of the string " << input << " = " << hash << std::endl;

        if (input == "exit") {
            break;
        }
    }

    return 0;
}