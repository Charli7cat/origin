#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <memory>

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

class TCPClient {
public:
    TCPClient();

    ~TCPClient();

    bool connectToServer(const std::string& host, int port);

    bool sendMessage(const std::string& message);

    std::string receiveMessage(int timeout_seconds = 5);

    void disconnect();

    bool isConnected() const;

    std::string getLocalInfo() const;

private:
    SOCKET_TYPE socket_fd;
    struct sockaddr_in server_addr;
    bool connected;

#ifdef _WIN32
    WSADATA wsaData;
    bool wsa_initialized;
#endif

    bool initNetwork();

    void cleanupNetwork();
};

#endif