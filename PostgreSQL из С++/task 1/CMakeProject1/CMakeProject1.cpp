#include "ClientManager.h"
#include <iostream>
#include <iomanip>

void printClient(const Client& client) {
}

void printAllClients(const std::vector<Client>& clients) {
}

int main() {
    try {
        ClientManager manager("clients.db", true);

        std::cout << "Создание таблиц базы данных..." << std::endl;
        manager.createTables();
        std::cout << "Таблицы успешно созданы!" << std::endl;


        std::cout << "\n12. Проверка существования телефона:" << std::endl;
        bool exists = manager.hasPhoneNumber("+79001234567");
        std::cout << "Телефон +79001234567 " << (exists ? "существует" : "не существует") << std::endl;

        exists = manager.hasPhoneNumber("+79999999999");
        std::cout << "Телефон +79999999999 " << (exists ? "существует" : "не существует") << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}