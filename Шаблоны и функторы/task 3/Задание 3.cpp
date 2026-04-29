#include <iostream>
#include <vector>
#include <sstream>

class DivisibleByThreeCounter {
private:
    int sum;
    int count;

public:
    DivisibleByThreeCounter() : sum(0), count(0) {}

    void operator()(int number) {
        if (number % 3 == 0) {
            sum += number;
            count++;
        }
    }

    int get_sum() const {
        return sum;
    }

    int get_count() const {
        return count;
    }
};

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss(input);

    DivisibleByThreeCounter counter;
    int num;

    while (iss >> num) {
        counter(num);
    }

    std::cout << "[OUT]: get_sum() = " << counter.get_sum() << std::endl;
    std::cout << "[OUT]: get_count() = " << counter.get_count() << std::endl;

    return 0;
}