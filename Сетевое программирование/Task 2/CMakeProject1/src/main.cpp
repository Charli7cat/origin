#include "tcp_server.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void printMenu() {
    std::cout << "\n=== TCP Сервер (Многопоточный) ===" << std::endl;
    std::cout << "1. Запустить сервер" << std::endl;
    std::cout << "2. Остановить сервер" << std::endl;
    std::cout << "3. Статистика" << std::endl;
    std::cout << "4. Выйти" << std::endl;
    std::cout << "Выберите действие: ";
}

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    TCPServer server;
    bool running = true;
    int port = 12345;

    std::cout << "=== TCP Сервер ===" << std::endl;
    std::cout << "Версия: 2.0 (Многопоточный)" << std::endl;
    std::cout << "Порт по умолчанию: " << port << std::endl;
    std::cout << std::endl;

    while (running) {
        printMenu();

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            if (server.isRunning()) {
                std::cout << "[WARN] Сервер уже запущен" << std::endl;
                continue;
            }

            std::cout << "Введите порт (по умолчанию " << port << "): ";
            std::string port_input;
            std::getline(std::cin, port_input);

            if (!port_input.empty()) {
                try {
                    port = std::stoi(port_input);
                }
                catch (...) {
                    std::cout << "[ERROR] Неверный порт, использую " << port << std::endl;
                }
            }

            std::thread server_thread([&server, port]() {
                server.start(port);
                });
            server_thread.detach();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        }
        else if (choice == "2") {
            if (!server.isRunning()) {
                std::cout << "[WARN] Сервер не запущен" << std::endl;
                continue;
            }
            server.stop();

        }
        else if (choice == "3") {
            std::cout << "\n=== Статистика ===" << std::endl;
            std::cout << "Статус: " << (server.isRunning() ? "Запущен" : "Остановлен") << std::endl;
            std::cout << "Активных подключений: " << server.getActiveConnections() << std::endl;
            std::cout << "Всего подключений: " << server.getTotalConnections() << std::endl;
            std::cout << std::endl;

        }
        else if (choice == "4") {
            if (server.isRunning()) {
                server.stop();
            }
            running = false;
            std::cout << "[INFO] Выход..." << std::endl;

        }
        else {
            std::cout << "[ERROR] Неверный выбор" << std::endl;
        }
    }

    return 0;
}