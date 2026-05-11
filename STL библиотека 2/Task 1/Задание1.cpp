#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>

template<typename T>
std::vector<T> removeDuplicates(const std::vector<T>& input) {
    std::unordered_set<T> seen;
    std::vector<T> result;

    for (const auto& item : input) {
        if (seen.find(item) == seen.end()) {
            seen.insert(item);
            result.push_back(item);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

template<typename T>
void printVector(const std::vector<T>& vec, const std::string& prefix = "") {
    std::cout << prefix;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) std::cout << " ";
        std::cout << vec[i];
    }
    std::cout << std::endl;
}

int main() {
    std::string inputLine;
    std::cout << "¬ведите числа через пробел: ";
    std::getline(std::cin, inputLine);

    std::vector<int> numbers;
    std::istringstream iss(inputLine);
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }

    std::cout << "[IN]: ";
    printVector(numbers);

    std::vector<int> uniqueNumbers = removeDuplicates(numbers);

    std::cout << "[OUT]: ";
    printVector(uniqueNumbers);

    return 0;
}