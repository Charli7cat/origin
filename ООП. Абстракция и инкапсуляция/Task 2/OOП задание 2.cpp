#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// Структура для хранения адреса
struct Address {
    std::string city;
    std::string street;
    std::string house;
    std::string apartment;
};

// Функция для сравнения двух адресов по названию города
bool compareByCity(const Address& a, const Address& b) {
    return a.city < b.city;
}

int main() {
    // Открываем файлы для чтения и записи
    std::ifstream input("in.txt");
    std::ofstream output("out.txt");

    // Проверяем, открылись ли файлы
    if (!input.is_open()) {
        std::cerr << "Error: could not open the file in.txt" << std::endl;
        return 1;
    }

    if (!output.is_open()) {
        std::cerr << "Error: Could not open out.txt file" << std::endl;
        input.close();
        return 1;
    }

    
    int n;
    input >> n;

    
    input.ignore();

    
    std::vector<Address> addresses;
    addresses.reserve(n);

    
    for (int i = 0; i < n; ++i) {
        Address addr;

        std::getline(input, addr.city);
        std::getline(input, addr.street);
        std::getline(input, addr.house);
        std::getline(input, addr.apartment);

        addresses.push_back(addr);
    }

    
    std::sort(addresses.begin(), addresses.end(), compareByCity);

    
    output << n << std::endl;

    
    for (int i = 0; i < n; ++i) {
        output << addresses[i].city << ", "
            << addresses[i].street << ", "
            << addresses[i].house << ", "
            << addresses[i].apartment;

        
        if (i < n - 1) {
            output << std::endl;
        }
    }

    
    input.close();
    output.close();

    std::cout << "The addresses were successfully sorted and written to out.txt" << std::endl;

    return 0;
}