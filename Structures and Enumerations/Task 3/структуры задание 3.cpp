#include <iostream>
#include <string>


struct Address {
    std::string city;     
    std::string street;   
    int houseNumber;      
    int apartmentNumber;  
    int postalCode;       
};


void printAddress(const Address& address) {
    std::cout << "City: " << address.city << std::endl;
    std::cout << "Street: " << address.street << std::endl;
    std::cout << "House number: " << address.houseNumber << std::endl;
    std::cout << "Apartment number: " << address.apartmentNumber << std::endl;
    std::cout << "Index: " << address.postalCode << std::endl;
    std::cout << std::endl;  
}

int main() {
    std::cout << "=== TESTING OF ADDRESS STRUCTURE ===" << std::endl;
    std::cout << std::endl;

    
    Address address1 = {
        "Moscow",      
        "Arbat",       
        12,            
        8,            
        123456         
    };

    std::cout << "Address 1:" << std::endl;
    printAddress(address1);

   
    Address address2;
    address2.city = "Izhevsk";
    address2.street = "Pushkin's";
    address2.houseNumber = 59;
    address2.apartmentNumber = 143;
    address2.postalCode = 953769;

    std::cout << "Address 2:" << std::endl;
    printAddress(address2);

   
    Address address3;
    address3.city = "Saint Petersburg";
    address3.street = "Nevsky Prospekt";
    address3.houseNumber = 25;
    address3.apartmentNumber = 42;
    address3.postalCode = 191186;

    std::cout << "Address 3:" << std::endl;
    printAddress(address3);

    
    Address address4 = { "Yekaterinburg", "Lenina", 45, 7, 620000};

    std::cout << "Address 4:" << std::endl;
    printAddress(address4);

    
    std::cout << "=== PRINTING OF THE ADDRESS ARRAY ===" << std::endl;
    Address addresses[3] = {
        {"Novosibirsk", "Krasny Prospekt", 10, 15, 630000},
        {"Kazan", "Bauman", 30, 22, 420000},
        {"Vladivostok", "Svetlanskaya", 50, 3, 690000}
    };

    for (int i = 0; i < 3; i++) {
        std::cout << "Address " << i + 1 << " in the array:" << std::endl;
        printAddress(addresses[i]);
    }

    EXIT_SUCCESS;
}