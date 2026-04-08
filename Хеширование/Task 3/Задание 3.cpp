#include <iostream>
#include <string>
#include <cstdint>

uint64_t hash_string(const std::string& str, uint64_t p, uint64_t n) {
    uint64_t hash = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        hash = (hash * p + static_cast<uint64_t>(str[i])) % n;
    }

    return hash;
}

int find_substring_light_rabin_karp(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return 0;
    if (pattern.length() > text.length()) return -1;

    const uint64_t p = 131;
    const uint64_t n = 1000000007;

    uint64_t pattern_hash = hash_string(pattern, p, n);

    uint64_t text_hash = hash_string(text.substr(0, pattern.length()), p, n);

    if (text_hash == pattern_hash && text.substr(0, pattern.length()) == pattern) {
        return 0;
    }

    uint64_t pow_p = 1;
    for (size_t i = 0; i < pattern.length() - 1; ++i) {
        pow_p = (pow_p * p) % n;
    }

    for (size_t i = 1; i <= text.length() - pattern.length(); ++i) {
        uint64_t old_char = static_cast<uint64_t>(text[i - 1]);
        uint64_t new_char = static_cast<uint64_t>(text[i + pattern.length() - 1]);

        text_hash = (text_hash + n - (old_char * pow_p) % n) % n;
        text_hash = (text_hash * p) % n;
        text_hash = (text_hash + new_char) % n;

        if (text_hash == pattern_hash) {
            if (text.substr(i, pattern.length()) == pattern) {
                return static_cast<int>(i);
            }
        }
    }

    return -1;
}

int main() {
    std::string search_string;
    std::string substring;

    std::cout << "Enter the search string: ";
    std::getline(std::cin, search_string);

    while (true) {
        std::cout << "Enter the substring you want to search for: ";
        std::getline(std::cin, substring);

        int index = find_substring_light_rabin_karp(search_string, substring);

        if (index != -1) {
            std::cout << "Substring " << substring << " found by index " << index << std::endl;
        }
        else {
            std::cout << "Substring " << substring << " not found" << std::endl;
        }

        if (substring == "exit") {
            break;
        }
    }

    return 0;
}