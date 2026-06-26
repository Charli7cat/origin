#include "ClientManager.h"
#include <iostream>
#include <iomanip>

void printClient(const Client& client) {
    std::cout << "\n=== Информация о клиенте ===" << std::endl;
    std::cout << "ID: " << client.id << std::endl;
    std::cout << "Имя: " << client.firstName << " " << client.lastName << std::endl;
    std::cout << "Email: " << client.email << std::endl;
    std::cout << "Создан: " << client.createdAt << std::endl;
    std::cout << "Обновлен: " << client.updatedAt << std::endl;

    if (!client.phones.empty()) {
        std::cout << "Телефоны:" << std::endl;
        for (const auto& phone : client.phones) {
            std::cout << "  - " << phone.number
                << " (" << phone.type << ")"
                << (phone.isPrimary ? " [ОСНОВНОЙ]" : "")
                << std::endl;
        }
    }
    else {
        std::cout << "Нет зарегистрированных телефонов." << std::endl;
    }
    std::cout << "================================" << std::endl;
}

void printAllClients(const std::vector<Client>& clients) {
    if (clients.empty()) {
        std::cout << "Клиенты не найдены." << std::endl;
        return;
    }

    std::cout << "\n=== Все клиенты (" << clients.size() << ") ===" << std::endl;
    for (const auto& client : clients) {
        std::cout << client.id << ". " << client.firstName << " " << client.lastName
            << " (" << client.email << ")" << std::endl;
        for (const auto& phone : client.phones) {
            std::cout << "    Телефон: " << phone.number
                << (phone.isPrimary ? " [ОСНОВНОЙ]" : "") << std::endl;
        }
    }
    std::cout << "================================" << std::endl;
}

int main() {
    try {
        ClientManager manager("clients.db");

        std::cout << "Создание таблиц базы данных..." << std::endl;
        if (!manager.createTables()) {
            std::cerr << "Не удалось создать таблицы!" << std::endl;
            return 1;
        }
        std::cout << "Таблицы успешно созданы!" << std::endl;

        std::cout << "\n1. Добавление клиентов..." << std::endl;
        manager.addClient("Иван", "Петров", "ivan.petrov@email.com");
        manager.addClient("Мария", "Иванова", "maria.ivanova@email.com");
        manager.addClient("Петр", "Сидоров", "petr.sidorov@email.com");
        std::cout << "Добавлено 3 клиента." << std::endl;

        std::cout << "\n2. Добавление телефонов..." << std::endl;
        manager.addPhone(1, "+79001234567", "mobile", true);
        manager.addPhone(1, "+79001234568", "home");
        manager.addPhone(1, "+79001234569", "work");
        manager.addPhone(2, "+79009876543", "mobile", true);
        std::cout << "Телефоны добавлены." << std::endl;

        std::cout << "\n3. Поиск по имени 'Иван'..." << std::endl;
        auto results = manager.findClients("Иван");
        for (const auto& client : results) {
            printClient(client);
        }

        std::cout << "\n4. Поиск по телефону '+79009876543'..." << std::endl;
        results = manager.findClients("79009876543");
        for (const auto& client : results) {
            printClient(client);
        }

        std::cout << "\n5. Обновление клиента (ID: 1)..." << std::endl;
        manager.updateClient(1, "Иван", "Петрович", "ivan.petrovich@email.com");
        std::cout << "Клиент обновлен." << std::endl;

        std::cout << "\n6. Установка основного телефона для клиента (ID: 1)..." << std::endl;
        manager.setPrimaryPhone(1, 2);
        std::cout << "Основной телефон изменен." << std::endl;

        std::cout << "\n7. Обновленная информация о клиенте (ID: 1):" << std::endl;
        Client* client = manager.findClientById(1);
        if (client) {
            printClient(*client);
            delete client;
        }

        std::cout << "\n8. Удаление телефона (ID: 3)..." << std::endl;
        manager.deletePhone(3);
        std::cout << "Телефон удален." << std::endl;

        std::cout << "\n9. Список всех клиентов:" << std::endl;
        auto allClients = manager.findAllClients();
        printAllClients(allClients);

        std::cout << "\n10. Удаление клиента (ID: 3)..." << std::endl;
        manager.deleteClient(3);
        std::cout << "Клиент удален." << std::endl;

        std::cout << "\n11. Финальный список клиентов:" << std::endl;
        allClients = manager.findAllClients();
        printAllClients(allClients);

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}