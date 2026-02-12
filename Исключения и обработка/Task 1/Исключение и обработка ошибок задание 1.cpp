#include <iostream>
#include <string>

class bad_length {};

int function(std::string str, int forbidden_length) {
    int len = str.length();
    if (len == forbidden_length) {
        throw bad_length();
    }
    return len;
}

int main() {
    int forbidden_length;

    std::cout << "Enter the forbidden length: ";
    std::cin >> forbidden_length;

    std::string word;

    while (true) {
        std::cout << "Enter the word: ";
        std::cin >> word;

        try {
            int length = function(word, forbidden_length);
            std::cout << "Word length \"" << word << "\" equal to " << length << std::endl;
        }
        catch (const bad_length&) {
            std::cout << "You have entered a word of forbidden length! Goodbye" << std::endl;
            break;
        }
    }

    return 0;
}