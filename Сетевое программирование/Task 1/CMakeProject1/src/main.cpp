#include "tcp_client.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void printMenu() {
    std::cout << "\n--- Меню ---" << std::endl;
    std::cout << "1. Отправить тестовое сообщение" << std::endl;
    std::cout << "2. Отправить пользовательское сообщение" << std::endl;
    std::cout << "3. Получить ответ от сервера" << std::endl;
    std::cout << "4. Информация о подключении" << std::endl;
    std::cout << "5. Выйти" << std::endl;
    std::cout << "Выберите действие: ";
}

std::string getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    std::cout << "=== TCP Клиент (с .h/.cpp) ===" << std::endl;
    std::cout << "Версия: 2.0" << std::endl;
    std::cout << std::endl;

    TCPClient client;

    std::string host = getUserInput("Введите IP-адрес сервера (по умолчанию 127.0.0.1): ");
    if (host.empty()) host = "127.0.0.1";

    std::string port_str = getUserInput("Введите порт сервера (по умолчанию 12345): ");
    int port = port_str.empty() ? 12345 : std::stoi(port_str);

    std::cout << std::endl;

    if (!client.connectToServer(host, port)) {
        std::cout << "\nНажмите Enter для выхода...";
        std::cin.get();
        return 1;
    }

    std::cout << "\nЛокальный адрес: " << client.getLocalInfo() << std::endl;
    std::cout << std::endl;

    bool running = true;
    while (running && client.isConnected()) {
        printMenu();

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::string test_msg = "Привет, сервер! Это тестовое сообщение.";
            if (client.sendMessage(test_msg)) {
                client.receiveMessage();
            }
        }
        else if (choice == "2") {
            std::string user_msg = getUserInput("Введите сообщение: ");
            if (!user_msg.empty()) {
                if (client.sendMessage(user_msg)) {
                    client.receiveMessage();
                }
            }
        }
        else if (choice == "3") {
            client.receiveMessage();
        }
        else if (choice == "4") {
            std::cout << "Статус: " << (client.isConnected() ? "Подключен" : "Отключен") << std::endl;
            std::cout << "Локальный адрес: " << client.getLocalInfo() << std::endl;
        }
        else if (choice == "5") {
            running = false;
            std::cout << "Выход..." << std::endl;
        }
        else {
            std::cout << "Неверный выбор" << std::endl;
        }
    }

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();
    return 0;
}