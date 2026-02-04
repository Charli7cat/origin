#include <iostream>
#include <string>
#include "Counter.h"

int main() {
    std::string answer;
    int initialValue = 1;

    std::cout << "Do you want to specify the initial value of the counter? Enter yes or no: ";
    std::cin >> answer;

    Counter counter;

    if (answer == "yes" || answer == "Yes" || answer == "YES" || answer == "y") {
        std::cout << "Enter the initial value of the counter: ";
        std::cin >> initialValue;
        counter = Counter(initialValue);
    }

    char command;

    do {
        std::cout << "Enter a command ('+', '-', '=', or 'x'): ";
        std::cin >> command;

        switch (command) {
        case '+':
            counter.increment();
            break;
        case '-':
            counter.decrement();
            break;
        case '=':
            std::cout << counter.getValue() << std::endl;
            break;
        case 'x':
            std::cout << "Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Unknown command! Try again." << std::endl;
        }
    } while (command != 'x');

    EXIT_SUCCESS;
}