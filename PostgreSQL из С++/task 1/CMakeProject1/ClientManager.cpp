#include "ClientManager.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

ClientManager::ClientManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Не удалось открыть базу данных: " + std::string(sqlite3_errmsg(db)));
    }
}

ClientManager::~ClientManager() {
    if (db) {
        sqlite3_close(db);
    }
}

bool ClientManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int ClientManager::getLastInsertId() {
    return sqlite3_last_insert_rowid(db);
}

bool ClientManager::createTables() {
    const std::string createClientsTable =
        "CREATE TABLE IF NOT EXISTS clients ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "first_name VARCHAR(50) NOT NULL,"
        "last_name VARCHAR(50) NOT NULL,"
        "email VARCHAR(100) UNIQUE NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");";

    const std::string createPhonesTable =
        "CREATE TABLE IF NOT EXISTS phones ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "client_id INTEGER NOT NULL,"
        "phone_number VARCHAR(20) NOT NULL,"
        "phone_type VARCHAR(20) DEFAULT 'mobile',"
        "is_primary BOOLEAN DEFAULT 0,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE CASCADE,"
        "UNIQUE(client_id, phone_number)"
        ");";

    const std::string createIndexes =
        "CREATE INDEX IF NOT EXISTS idx_clients_email ON clients(email);"
        "CREATE INDEX IF NOT EXISTS idx_clients_name ON clients(first_name, last_name);"
        "CREATE INDEX IF NOT EXISTS idx_phones_number ON phones(phone_number);"
        "CREATE INDEX IF NOT EXISTS idx_phones_client ON phones(client_id);";

    return executeQuery(createClientsTable) &&
        executeQuery(createPhonesTable) &&
        executeQuery(createIndexes);
}

bool ClientManager::addClient(const std::string& firstName, const std::string& lastName,
    const std::string& email) {
    std::stringstream query;
    query << "INSERT INTO clients (first_name, last_name, email) VALUES ('"
        << firstName << "', '" << lastName << "', '" << email << "');";
    return executeQuery(query.str());
}

bool ClientManager::addPhone(int clientId, const std::string& phoneNumber,
    const std::string& phoneType, bool isPrimary) {
    Client* client = findClientById(clientId);
    if (!client) {
        std::cerr << "Клиент с ID " << clientId << " не найден." << std::endl;
        return false;
    }
    delete client;

    if (hasPhoneNumber(clientId, phoneNumber)) {
        std::cerr << "Номер телефона уже существует для этого клиента." << std::endl;
        return false;
    }

    if (isPrimary) {
        std::stringstream updateQuery;
        updateQuery << "UPDATE phones SET is_primary = 0 WHERE client_id = " << clientId << ";";
        executeQuery(updateQuery.str());
    }

    std::stringstream query;
    query << "INSERT INTO phones (client_id, phone_number, phone_type, is_primary) VALUES ("
        << clientId << ", '" << phoneNumber << "', '" << phoneType << "', "
        << (isPrimary ? 1 : 0) << ");";
    return executeQuery(query.str());
}

bool ClientManager::updateClient(int clientId, const std::string& firstName,
    const std::string& lastName, const std::string& email) {
    std::stringstream query;
    query << "UPDATE clients SET first_name = '" << firstName
        << "', last_name = '" << lastName
        << "', email = '" << email
        << "', updated_at = CURRENT_TIMESTAMP WHERE id = " << clientId << ";";
    return executeQuery(query.str());
}

bool ClientManager::deletePhone(int phoneId) {
    std::stringstream query;
    query << "DELETE FROM phones WHERE id = " << phoneId << ";";
    return executeQuery(query.str());
}

bool ClientManager::deleteClient(int clientId) {
    std::stringstream query;
    query << "DELETE FROM clients WHERE id = " << clientId << ";";
    return executeQuery(query.str());
}

Client* ClientManager::fetchClientFromStatement(sqlite3_stmt* stmt) {
    Client* client = new Client();
    client->id = sqlite3_column_int(stmt, 0);
    client->firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    client->lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    client->email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    client->createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    client->updatedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    return client;
}

void ClientManager::fetchPhonesForClient(Client& client) {
    std::stringstream query;
    query << "SELECT id, phone_number, phone_type, is_primary, created_at FROM phones "
        << "WHERE client_id = " << client.id << " ORDER BY is_primary DESC, created_at;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client::Phone phone;
        phone.id = sqlite3_column_int(stmt, 0);
        phone.number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        phone.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        phone.isPrimary = sqlite3_column_int(stmt, 3) != 0;
        phone.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        client.phones.push_back(phone);
    }
    sqlite3_finalize(stmt);
}

std::vector<Client> ClientManager::findClients(const std::string& searchTerm) {
    std::vector<Client> results;

    std::stringstream query;
    query << "SELECT id, first_name, last_name, email, created_at, updated_at FROM clients "
        << "WHERE first_name LIKE '%" << searchTerm << "%' "
        << "OR last_name LIKE '%" << searchTerm << "%' "
        << "OR email LIKE '%" << searchTerm << "%' "
        << "OR id IN (SELECT client_id FROM phones WHERE phone_number LIKE '%" << searchTerm << "%') "
        << "ORDER BY last_name, first_name;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось подготовить запрос: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::unique_ptr<Client> client(fetchClientFromStatement(stmt));
        fetchPhonesForClient(*client);
        results.push_back(*client);
    }

    sqlite3_finalize(stmt);
    return results;
}

Client* ClientManager::findClientById(int clientId) {
    std::stringstream query;
    query << "SELECT id, first_name, last_name, email, created_at, updated_at FROM clients "
        << "WHERE id = " << clientId << ";";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось подготовить запрос: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    Client* client = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        client = fetchClientFromStatement(stmt);
        fetchPhonesForClient(*client);
    }

    sqlite3_finalize(stmt);
    return client;
}

std::vector<Client> ClientManager::findAllClients() {
    return findClients("");
}

bool ClientManager::setPrimaryPhone(int clientId, int phoneId) {
    std::stringstream resetQuery;
    resetQuery << "UPDATE phones SET is_primary = 0 WHERE client_id = " << clientId << ";";
    if (!executeQuery(resetQuery.str())) {
        return false;
    }

    std::stringstream setQuery;
    setQuery << "UPDATE phones SET is_primary = 1 WHERE id = " << phoneId
        << " AND client_id = " << clientId << ";";
    return executeQuery(setQuery.str());
}

std::vector<Client::Phone> ClientManager::getClientPhones(int clientId) {
    std::vector<Client::Phone> phones;
    std::stringstream query;
    query << "SELECT id, phone_number, phone_type, is_primary, created_at FROM phones "
        << "WHERE client_id = " << clientId << " ORDER BY is_primary DESC, created_at;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return phones;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client::Phone phone;
        phone.id = sqlite3_column_int(stmt, 0);
        phone.number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        phone.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        phone.isPrimary = sqlite3_column_int(stmt, 3) != 0;
        phone.createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        phones.push_back(phone);
    }

    sqlite3_finalize(stmt);
    return phones;
}

bool ClientManager::hasPhoneNumber(int clientId, const std::string& phoneNumber) {
    std::stringstream query;
    query << "SELECT COUNT(*) FROM phones WHERE client_id = " << clientId
        << " AND phone_number = '" << phoneNumber << "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}