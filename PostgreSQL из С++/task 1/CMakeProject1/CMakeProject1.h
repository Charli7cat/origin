#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <libpq-fe.h>

struct Client {
    int id;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string createdAt;
    std::string updatedAt;

    struct Phone {
        int id;
        std::string number;
        std::string type;
        bool isPrimary;
        std::string createdAt;
    };
    std::vector<Phone> phones;
};

class ClientManager {
private:
    PGconn* conn;
    std::string connInfo;

    bool executeQuery(const std::string& query);
    PGresult* executeQueryWithResult(const std::string& query);
    int getLastInsertId();
    Client* fetchClientFromResult(PGresult* res, int row);
    void fetchPhonesForClient(Client& client);
    std::string escapeString(const std::string& str);

public:
    ClientManager(const std::string& connInfo);
    ~ClientManager();

    bool createTables();
    bool addClient(const std::string& firstName, const std::string& lastName,
        const std::string& email);
    bool addPhone(int clientId, const std::string& phoneNumber,
        const std::string& phoneType = "mobile", bool isPrimary = false);
    bool updateClient(int clientId, const std::string& firstName,
        const std::string& lastName, const std::string& email);
    bool deletePhone(int phoneId);
    bool deleteClient(int clientId);

    std::vector<Client> findClients(const std::string& searchTerm);
    Client* findClientById(int clientId);
    std::vector<Client> findAllClients();

    bool setPrimaryPhone(int clientId, int phoneId);
    std::vector<Client::Phone> getClientPhones(int clientId);
    bool hasPhoneNumber(int clientId, const std::string& phoneNumber);
};

#endif