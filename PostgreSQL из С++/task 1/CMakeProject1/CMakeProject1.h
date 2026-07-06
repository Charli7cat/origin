#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <sqlite3.h>

struct Phone {
    int id;
    int clientId;
    std::string number;
    std::string type;
    bool isPrimary;
};

struct Client {
    int id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string createdAt;
    std::string updatedAt;
    std::vector<Phone> phones;
};

class ClientManager {
public:
    explicit ClientManager(const std::string& dbPath, bool enableLogging = false);
    ~ClientManager();

    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    void addClient(const std::string& firstName, const std::string& lastName,
        const std::string& email);
    void updateClient(int clientId, const std::string& firstName,
        const std::string& lastName, const std::string& email);
    void deleteClient(int clientId);

    Client* findClientById(int clientId);
    std::vector<Client> findClients(const std::string& searchTerm);
    std::vector<Client> findAllClients();

    void addPhone(int clientId, const std::string& number,
        const std::string& type = "mobile", bool isPrimary = false);
    void deletePhone(int phoneId);
    void setPrimaryPhone(int clientId, int phoneId);
    bool hasPhoneNumber(const std::string& number);

    bool createTables();

private:
    sqlite3* db;
    bool loggingEnabled;

    void log(const std::string& message) const;
    void executeQuery(const std::string& query);
    void checkDbError(int rc, const std::string& context) const;

    Client* clientFromStatement(sqlite3_stmt* stmt);
    std::vector<Phone> loadPhonesForClient(int clientId);
    void loadPhonesForClients(std::vector<Client>& clients);
    bool clientExists(int clientId);
};

#endif