#include "tcp_server.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <chrono>
#include <ctime>

TCPServer::TCPServer()
    : server_socket(INVALID_SOCKET_TYPE)
    , running(false)
    , total_connections(0)
    , active_connections(0)
#ifdef _WIN32
    , wsa_initialized(false)
#endif
{
    initNetwork();
}

TCPServer::~TCPServer() {
    stop();
    cleanupNetwork();
}

bool TCPServer::initNetwork() {
#ifdef _WIN32
    if (!wsa_initialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
            wsa_initialized = true;
            std::cout << "[INFO] Winsock инициализирован" << std::endl;
            return true;
        }
        else {
            std::cerr << "[ERROR] Ошибка инициализации Winsock" << std::endl;
            return false;
        }
    }
    return true;
#else
    return true;
#endif
}

void TCPServer::cleanupNetwork() {
#ifdef _WIN32
    if (wsa_initialized) {
        WSACleanup();
        wsa_initialized = false;
        std::cout << "[INFO] Winsock очищен" << std::endl;
    }
#endif
}

bool TCPServer::start(int port) {
    if (running) {
        std::cout << "[WARN] Сервер уже запущен" << std::endl;
        return false;
    }

    if (!initNetwork()) {
        return false;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET_TYPE) {
        std::cerr << "[ERROR] Не удалось создать сокет" << std::endl;
        return false;
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR_TYPE) {
        std::cerr << "[ERROR] Ошибка привязки сокета к порту " << port << std::endl;
        CLOSE_SOCKET(server_socket);
        server_socket = INVALID_SOCKET_TYPE;
        return false;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR_TYPE) {
        std::cerr << "[ERROR] Ошибка при начале прослушивания" << std::endl;
        CLOSE_SOCKET(server_socket);
        server_socket = INVALID_SOCKET_TYPE;
        return false;
    }

    running = true;
    std::cout << "[INFO] Сервер запущен на порту " << port << std::endl;
    std::cout << "[INFO] Ожидание подключений..." << std::endl;

    while (running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        SOCKET_TYPE client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);

        if (client_socket == INVALID_SOCKET_TYPE) {
            if (running) {
                std::cerr << "[ERROR] Ошибка принятия соединения" << std::endl;
            }
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(client_addr.sin_port);

        std::cout << "[INFO] Новое подключение от " << client_ip << ":" << client_port << std::endl;

        {
            std::lock_guard<std::mutex> lock(stats_mutex);
            total_connections++;
            active_connections++;
        }

        std::thread client_thread(&TCPServer::handleClient, this, client_socket);
        client_thread.detach();
    }

    return true;
}

void TCPServer::stop() {
    if (!running) {
        return;
    }

    running = false;
    std::cout << "[INFO] Остановка сервера..." << std::endl;

    if (server_socket != INVALID_SOCKET_TYPE) {
        CLOSE_SOCKET(server_socket);
        server_socket = INVALID_SOCKET_TYPE;
    }

    std::cout << "[INFO] Сервер остановлен" << std::endl;
}

bool TCPServer::isRunning() const {
    return running;
}

void TCPServer::handleClient(SOCKET_TYPE client_socket) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];

    if (getpeername(client_socket, (struct sockaddr*)&client_addr, &addr_len) == 0) {
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    }
    else {
        strcpy(client_ip, "unknown");
    }

    std::cout << "[INFO] Поток для клиента " << client_ip << " запущен" << std::endl;

    while (running) {
        std::string message = receiveMessage(client_socket);

        if (message.empty()) {
            break;
        }

        std::cout << "[RECV] От " << client_ip << ": " << message << std::endl;

        std::string response = "Сервер получил: \"" + message + "\"";

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string time_str = std::ctime(&time);
        time_str.pop_back();

        response += " (время: " + time_str + ")";

        if (!sendResponse(client_socket, response)) {
            std::cout << "[ERROR] Ошибка отправки ответа клиенту " << client_ip << std::endl;
            break;
        }

        std::cout << "[SEND] Клиенту " << client_ip << ": " << response << std::endl;
    }

    CLOSE_SOCKET(client_socket);

    active_connections--;
    std::cout << "[INFO] Клиент " << client_ip << " отключился. Активных подключений: "
        << active_connections.load() << std::endl;
}

std::string TCPServer::receiveMessage(SOCKET_TYPE client_socket) {
    char buffer[4096] = { 0 };

#ifdef _WIN32
    int timeout = 5000;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#else
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
#endif

    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        return std::string(buffer);
    }
    else if (bytes_received == 0) {
        return "";
    }
    else {
        return "";
    }
}

bool TCPServer::sendResponse(SOCKET_TYPE client_socket, const std::string& response) {
    int bytes_sent = send(client_socket, response.c_str(), static_cast<int>(response.length()), 0);
    return bytes_sent > 0;
}

size_t TCPServer::getActiveConnections() const {
    return active_connections.load();
}

size_t TCPServer::getTotalConnections() const {
    std::lock_guard<std::mutex> lock(stats_mutex);
    return total_connections;
}