#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Address {
private:
    std::string city;
    std::string street;
    std::string house;
    std::string apartment;

public:
    Address() = default;

    Address(const std::string& city, const std::string& street,
        const std::string& house, const std::string& apartment)
        : city(city), street(street), house(house), apartment(apartment) {
    }

    void readFromStream(std::ifstream& input) {
        std::getline(input, city);
        std::getline(input, street);
        std::getline(input, house);
        std::getline(input, apartment);
    }

    void writeToStream(std::ofstream& output) const {
        output << city << ", " << street << ", " << house << ", " << apartment;
    }

    std::string getCity() const { return city; }
    std::string getStreet() const { return street; }
    std::string getHouse() const { return house; }
    std::string getApartment() const { return apartment; }
};

class AddressBook {
private:
    std::vector<Address> addresses;

public:
    void readFromFile(const std::string& filename) {
        std::ifstream input(filename);

        if (!input.is_open()) {
            throw std::runtime_error("Couldn't open the file " + filename);
        }

        int n;
        input >> n;
        input.ignore();

        addresses.resize(n);

        for (int i = 0; i < n; ++i) {
            addresses[i].readFromStream(input);
        }

        input.close();
    }

    void writeReversedToFile(const std::string& filename) const {
        std::ofstream output(filename);

        if (!output.is_open()) {
            throw std::runtime_error("Couldn't open the file " + filename);
        }

        output << addresses.size() << std::endl;

        for (int i = addresses.size() - 1; i >= 0; --i) {
            addresses[i].writeToStream(output);
            if (i > 0) {
                output << std::endl;
            }
        }

        output.close();
    }

    size_t getAddressCount() const {
        return addresses.size();
    }
    const Address& getAddress(size_t index) const {
        if (index >= addresses.size()) {
            throw std::out_of_range("Индекс выходит за границы массива");
        }
        return addresses[index];
    }

    void addAddress(const Address& address) {
        addresses.push_back(address);
    }
};

int main() {
    try {
        AddressBook addressBook;

        addressBook.readFromFile("in.txt");

        addressBook.writeReversedToFile("out.txt");

        std::cout << "The program has been successfully completed!" << std::endl;
        std::cout << "Addresses processed:" << addressBook.getAddressCount() << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error:" << e.what() << std::endl;
        return 1;
    }

    EXIT_SUCCESS;
}