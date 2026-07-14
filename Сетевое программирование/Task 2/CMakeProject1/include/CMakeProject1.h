#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
using SOCKET_TYPE = SOCKET;
#define INVALID_SOCKET_TYPE INVALID_SOCKET
#define SOCKET_ERROR_TYPE SOCKET_ERROR
#define CLOSE_SOCKET(s) closesocket(s)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using SOCKET_TYPE = int;
#define INVALID_SOCKET_TYPE -1
#define SOCKET_ERROR_TYPE -1
#define CLOSE_SOCKET(s) close(s)
#endif

class TCPServer {
public:
    TCPServer();
    ~TCPServer();

    bool start(int port);
    void stop();
    bool isRunning() const;

    size_t getActiveConnections() const;
    size_t getTotalConnections() const;

private:
    SOCKET_TYPE server_socket;
    std::atomic<bool> running;
    std::vector<std::thread> client_threads;
    mutable std::mutex threads_mutex;

    size_t total_connections;
    std::atomic<size_t> active_connections;
    mutable std::mutex stats_mutex;

#ifdef _WIN32
    WSADATA wsaData;
    bool wsa_initialized;
#endif

    void handleClient(SOCKET_TYPE client_socket);

    bool initNetwork();

    void cleanupNetwork();

    bool sendResponse(SOCKET_TYPE client_socket, const std::string& response);

    std::string receiveMessage(SOCKET_TYPE client_socket);
};

#endif