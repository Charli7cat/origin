#include "ClientManager.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <iomanip>

ClientManager::ClientManager(const std::string& dbPath, bool enableLogging)
    : db(nullptr), loggingEnabled(enableLogging) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::string error = "Не удалось открыть БД: " + std::string(sqlite3_errmsg(db));
        sqlite3_close(db);
        throw std::runtime_error(error);
    }
    log("База данных открыта: " + dbPath);
}

ClientManager::~ClientManager() {
    if (db) {
        sqlite3_close(db);
        log("База данных закрыта");
    }
}

void ClientManager::log(const std::string& message) const {
    if (loggingEnabled) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cout << "[LOG] " << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
            << " - " << message << std::endl;
    }
}

void ClientManager::checkDbError(int rc, const std::string& context) const {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
        std::string error = "Ошибка SQLite в " + context + ": " +
            std::string(sqlite3_errmsg(db));
        log(error);
        throw std::runtime_error(error);
    }
}

void ClientManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::string error = "Ошибка выполнения запроса: " + std::string(errMsg);
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

bool ClientManager::createTables() {
    try {
        const std::string createClients = R"(
            CREATE TABLE IF NOT EXISTS clients (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                first_name TEXT NOT NULL,
                last_name TEXT NOT NULL,
                email TEXT UNIQUE NOT NULL,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";

        const std::string createPhones = R"(
            CREATE TABLE IF NOT EXISTS phones (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                client_id INTEGER NOT NULL,
                number TEXT NOT NULL UNIQUE,
                type TEXT DEFAULT 'mobile',
                is_primary BOOLEAN DEFAULT 0,
                FOREIGN KEY (client_id) REFERENCES clients(id) ON DELETE CASCADE
            );
        )";

        const std::string createTrigger = R"(
            CREATE TRIGGER IF NOT EXISTS update_client_timestamp 
            AFTER UPDATE ON clients
            BEGIN
                UPDATE clients SET updated_at = CURRENT_TIMESTAMP WHERE id = NEW.id;
            END;
        )";

        executeQuery(createClients);
        executeQuery(createPhones);
        executeQuery(createTrigger);

        log("Таблицы успешно созданы");
        return true;
    }
    catch (const std::exception& e) {
        log("Ошибка создания таблиц: " + std::string(e.what()));
        throw;
    }
}

void ClientManager::addClient(const std::string& firstName, const std::string& lastName,
    const std::string& email) {
    std::string query = "INSERT INTO clients (first_name, last_name, email) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare addClient");

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Ошибка добавления клиента: " + std::string(sqlite3_errmsg(db)));
    }

    log("Добавлен клиент: " + firstName + " " + lastName);
}

void ClientManager::updateClient(int clientId, const std::string& firstName,
    const std::string& lastName, const std::string& email) {
    if (!clientExists(clientId)) {
        throw std::runtime_error("Клиент с ID " + std::to_string(clientId) + " не найден");
    }

    std::string query = "UPDATE clients SET first_name = ?, last_name = ?, email = ? WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare updateClient");

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, clientId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Ошибка обновления клиента: " + std::string(sqlite3_errmsg(db)));
    }

    log("Обновлен клиент ID: " + std::to_string(clientId));
}

void ClientManager::deleteClient(int clientId) {
    if (!clientExists(clientId)) {
        throw std::runtime_error("Клиент с ID " + std::to_string(clientId) + " не найден");
    }

    std::string query = "DELETE FROM clients WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare deleteClient");

    sqlite3_bind_int(stmt, 1, clientId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Ошибка удаления клиента: " + std::string(sqlite3_errmsg(db)));
    }

    log("Удален клиент ID: " + std::to_string(clientId));
}

Client* ClientManager::findClientById(int clientId) {
    std::string query = "SELECT * FROM clients WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare findClientById");

    sqlite3_bind_int(stmt, 1, clientId);

    Client* client = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        client = clientFromStatement(stmt);
        if (client) {
            client->phones = loadPhonesForClient(clientId);
        }
    }

    sqlite3_finalize(stmt);
    return client;
}

std::vector<Client> ClientManager::findClients(const std::string& searchTerm) {
    std::vector<Client> result;

    std::string query = R"(
        SELECT DISTINCT c.* FROM clients c
        LEFT JOIN phones p ON c.id = p.client_id
        WHERE c.first_name LIKE ? OR c.last_name LIKE ? OR c.email LIKE ? OR p.number LIKE ?
    )";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare findClients");

    std::string pattern = "%" + searchTerm + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, pattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, pattern.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client* client = clientFromStatement(stmt);
        if (client) {
            result.push_back(*client);
            delete client;
        }
    }

    sqlite3_finalize(stmt);

    loadPhonesForClients(result);

    return result;
}

std::vector<Client> ClientManager::findAllClients() {
    std::vector<Client> result;

    std::string query = "SELECT * FROM clients ORDER BY id";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare findAllClients");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client* client = clientFromStatement(stmt);
        if (client) {
            result.push_back(*client);
            delete client;
        }
    }

    sqlite3_finalize(stmt);
    loadPhonesForClients(result);

    return result;
}

void ClientManager::addPhone(int clientId, const std::string& number,
    const std::string& type, bool isPrimary) {
    if (!clientExists(clientId)) {
        throw std::runtime_error("Клиент с ID " + std::to_string(clientId) + " не найден");
    }

    std::string query = "INSERT INTO phones (client_id, number, type, is_primary) VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare addPhone");

    sqlite3_bind_int(stmt, 1, clientId);
    sqlite3_bind_text(stmt, 2, number.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, isPrimary ? 1 : 0);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Ошибка добавления телефона: " + std::string(sqlite3_errmsg(db)));
    }

    if (isPrimary) {
        setPrimaryPhone(clientId, sqlite3_last_insert_rowid(db));
    }

    log("Добавлен телефон для клиента ID: " + std::to_string(clientId));
}

void ClientManager::deletePhone(int phoneId) {
    std::string query = "DELETE FROM phones WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare deletePhone");

    sqlite3_bind_int(stmt, 1, phoneId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Ошибка удаления телефона: " + std::string(sqlite3_errmsg(db)));
    }

    if (sqlite3_changes(db) == 0) {
        throw std::runtime_error("Телефон с ID " + std::to_string(phoneId) + " не найден");
    }

    log("Удален телефон ID: " + std::to_string(phoneId));
}

void ClientManager::setPrimaryPhone(int clientId, int phoneId) {
    if (!clientExists(clientId)) {
        throw std::runtime_error("Клиент с ID " + std::to_string(clientId) + " не найден");
    }

    executeQuery("BEGIN TRANSACTION");

    try {
        std::string resetQuery = "UPDATE phones SET is_primary = 0 WHERE client_id = ?";
        sqlite3_stmt* resetStmt = nullptr;
        int rc = sqlite3_prepare_v2(db, resetQuery.c_str(), -1, &resetStmt, nullptr);
        checkDbError(rc, "prepare setPrimaryPhone reset");

        sqlite3_bind_int(resetStmt, 1, clientId);
        rc = sqlite3_step(resetStmt);
        sqlite3_finalize(resetStmt);

        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Ошибка сброса основных телефонов");
        }

        std::string setQuery = "UPDATE phones SET is_primary = 1 WHERE id = ? AND client_id = ?";
        sqlite3_stmt* setStmt = nullptr;
        rc = sqlite3_prepare_v2(db, setQuery.c_str(), -1, &setStmt, nullptr);
        checkDbError(rc, "prepare setPrimaryPhone set");

        sqlite3_bind_int(setStmt, 1, phoneId);
        sqlite3_bind_int(setStmt, 2, clientId);

        rc = sqlite3_step(setStmt);
        sqlite3_finalize(setStmt);

        if (rc != SQLITE_DONE) {
            throw std::runtime_error("Ошибка установки основного телефона");
        }

        if (sqlite3_changes(db) == 0) {
            throw std::runtime_error("Телефон с ID " + std::to_string(phoneId) +
                " не принадлежит клиенту " + std::to_string(clientId));
        }

        executeQuery("COMMIT");
        log("Установлен основной телефон ID: " + std::to_string(phoneId) +
            " для клиента ID: " + std::to_string(clientId));
    }
    catch (...) {
        executeQuery("ROLLBACK");
        throw;
    }
}

bool ClientManager::hasPhoneNumber(const std::string& number) {
    std::string query = "SELECT COUNT(*) FROM phones WHERE number = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare hasPhoneNumber");

    sqlite3_bind_text(stmt, 1, number.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

bool ClientManager::clientExists(int clientId) {
    std::string query = "SELECT COUNT(*) FROM clients WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare clientExists");

    sqlite3_bind_int(stmt, 1, clientId);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0;
    }

    sqlite3_finalize(stmt);
    return exists;
}

Client* ClientManager::clientFromStatement(sqlite3_stmt* stmt) {
    Client* client = new Client();
    client->id = sqlite3_column_int(stmt, 0);
    client->firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    client->lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    client->email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    client->createdAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    client->updatedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    return client;
}

std::vector<Phone> ClientManager::loadPhonesForClient(int clientId) {
    std::vector<Phone> phones;
    std::string query = "SELECT * FROM phones WHERE client_id = ?";
    sqlite3_stmt* stmt = nullptr;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    checkDbError(rc, "prepare loadPhonesForClient");

    sqlite3_bind_int(stmt, 1, clientId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Phone phone;
        phone.id = sqlite3_column_int(stmt, 0);
        phone.clientId = sqlite3_column_int(stmt, 1);
        phone.number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        phone.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        phone.isPrimary = sqlite3_column_int(stmt, 4) != 0;
        phones.push_back(phone);
    }

    sqlite3_finalize(stmt);
    return phones;
}

void ClientManager::loadPhonesForClients(std::vector<Client>& clients) {
    for (auto& client : clients) {
        client.phones = loadPhonesForClient(client.id);
    }
}