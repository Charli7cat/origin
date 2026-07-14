#include "tcp_client.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

TCPClient::TCPClient()
    : socket_fd(INVALID_SOCKET_TYPE)
    , connected(false)
#ifdef _WIN32
    , wsa_initialized(false)
#endif
{
    initNetwork();
}

TCPClient::~TCPClient() {
    disconnect();
    cleanupNetwork();
}

bool TCPClient::initNetwork() {
#ifdef _WIN32
    if (!wsa_initialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
            wsa_initialized = true;
            std::cout << "Winsock инициализирован" << std::endl;
            return true;
        }
        else {
            std::cerr << "Ошибка инициализации Winsock" << std::endl;
            return false;
        }
    }
    return true;
#else
    return true;
#endif
}

void TCPClient::cleanupNetwork() {
#ifdef _WIN32
    if (wsa_initialized) {
        WSACleanup();
        wsa_initialized = false;
        std::cout << "Winsock очищен" << std::endl;
    }
#endif
}

bool TCPClient::connectToServer(const std::string& host, int port) {
    if (!initNetwork()) {
        return false;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == INVALID_SOCKET_TYPE) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return false;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес: " << host << std::endl;
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET_TYPE;
        return false;
    }

    std::cout << "Подключение к " << host << ":" << port << "..." << std::endl;

    if (::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR_TYPE) {
        std::cerr << "Ошибка подключения" << std::endl;
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET_TYPE;
        return false;
    }

    connected = true;
    std::cout << "✓ Подключено успешно!" << std::endl;
    return true;
}

bool TCPClient::sendMessage(const std::string& message) {
    if (!connected) {
        std::cerr << "Не подключено к серверу" << std::endl;
        return false;
    }

    std::cout << "📤 Отправка: " << message << std::endl;

    int bytes_sent = send(socket_fd, message.c_str(), static_cast<int>(message.length()), 0);
    if (bytes_sent == SOCKET_ERROR_TYPE) {
        std::cerr << "Ошибка отправки данных" << std::endl;
        return false;
    }

    std::cout << "✓ Отправлено " << bytes_sent << " байт" << std::endl;
    return true;
}

std::string TCPClient::receiveMessage(int timeout_seconds) {
    if (!connected) {
        return "Ошибка: не подключено к серверу";
    }

    char buffer[4096] = { 0 };

#ifdef _WIN32
    int timeout = timeout_seconds * 1000;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = timeout_seconds;
    tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
#endif

    std::cout << "⏳ Ожидание ответа..." << std::endl;

    int bytes_received = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::string response(buffer);
        std::cout << "📥 Получен ответ: " << response << std::endl;
        std::cout << "✓ Получено " << bytes_received << " байт" << std::endl;
        return response;
    }
    else if (bytes_received == 0) {
        std::cout << "⚠ Сервер закрыл соединение" << std::endl;
        connected = false;
        return "Соединение закрыто сервером";
    }
    else {
        std::cerr << "Ошибка приема данных" << std::endl;
        return "Ошибка приема";
    }
}

void TCPClient::disconnect() {
    if (connected && socket_fd != INVALID_SOCKET_TYPE) {
        CLOSE_SOCKET(socket_fd);
        socket_fd = INVALID_SOCKET_TYPE;
        connected = false;
        std::cout << "Соединение закрыто" << std::endl;
    }
}

bool TCPClient::isConnected() const {
    return connected;
}

std::string TCPClient::getLocalInfo() const {
    if (!connected || socket_fd == INVALID_SOCKET_TYPE) {
        return "Не подключен";
    }

    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);

    if (getsockname(socket_fd, (struct sockaddr*)&local_addr, &addr_len) == 0) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &local_addr.sin_addr, ip, INET_ADDRSTRLEN);
        return std::string(ip) + ":" + std::to_string(ntohs(local_addr.sin_port));
    }

    return "Неизвестно";
}