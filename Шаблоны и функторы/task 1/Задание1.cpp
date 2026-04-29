#include <iostream>
#include <vector>
#include <sstream>

template <typename T>
T square(T value) {
    return value * value;
}

template <typename T>
std::vector<T> square(const std::vector<T>& vec) {
    std::vector<T> result;
    result.reserve(vec.size());
    for (const auto& val : vec) {
        result.push_back(val * val);
    }
    return result;
}

template <typename T>
void printVector(const std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "[IN]: 4" << std::endl;
    std::cout << "[OUT]: " << square(4) << std::endl;
    std::cout << std::endl;

    std::vector<int> input = { -1, 4, 8 };
    std::cout << "[IN]: ";
    printVector(input);

    std::vector<int> output = square(input);
    std::cout << "[OUT]: ";
    printVector(output);

    return 0;
}