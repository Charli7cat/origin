#include <iostream>
#include <string>
#include <array>

enum class Month {
     January = 1,
     February,
     March,
     April,
     May,
     June,
     July,
     August,
     September,
     October,
     November,
     December
};

int main() {
    std::array < std::string, 13 > monthNames = {
        "",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    int input; 

    std::cout << "=== Find out the name of the month ===" << std::endl;
    do {
        std::cout << "\nEnter the month number (1-12, 0 for exit): ";
        if (!(std::cin >> input)) {
            std::cout << "Input error! Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (input == 0) {
            std::cout << "Completion of work..." << std::endl;
            break;
        }

        if (input < 1 || input > 12) {
            std::cout << "Wrong number!" << std::endl;
            continue;
        }

        Month month = static_cast<Month>(input);
        std::cout << monthNames[static_cast<int>(month)] << std::endl;
    } while (true);
    std::cout << "\nGoodbye!" << std::endl;

    EXIT_SUCCESS;
}
