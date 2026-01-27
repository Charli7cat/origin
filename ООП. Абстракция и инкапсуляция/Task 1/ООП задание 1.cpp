#include <iostream>
#include <fstream>
#include <vector>
#include <string>


int main() {
    std::ifstream input("in.txt");
    std::ofstream output("out.txt");


    if (!input.is_open()) {
        std::cerr << "Error: could not open the file in.txt" << std::endl;
        return 1;
    }

    if (!output.is_open()) {
        std::cerr << "Error: could not open the file out.txt" << std::endl;
        input.close();
        return 1;
    }

    int n;
    input >> n;
    input.ignore();

    struct Address {
        std::string city;
        std::string street;
        std::string house;
        std::string apartment;
    };

    std::vector<Address> addresses;
    addresses.reserve(n);

    for (int i = 0; i < n; i++) {
        Address addr;
        std::getline(input, addr.city);
        std::getline(input, addr.street);
        std::getline(input, addr.house);
        std::getline(input, addr.apartment);
        addresses.push_back(addr);
    }

    output << n << std::endl;

    for (int i = n - 1; i >= 0; --i) {
        output << addresses[i].city << ", "
            << addresses[i].street << ", "
            << addresses[i].house << ", "
            << addresses[i].apartment;

        if (i > 0) {
            output << std::endl;
        }
    }

    input.close();
    output.close();

    std::cout << "The program was successfully executed! The result is written to out.txt" << std::endl;

    EXIT_SUCCESS;
}