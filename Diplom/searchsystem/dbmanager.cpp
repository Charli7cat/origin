#include "dbmanager.h"
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>

QSqlDatabase DbManager::db() {
    return QSqlDatabase::database("main");
}

bool DbManager::connect(const QString &iniPath, QString *error) {
    QSettings s(iniPath, QSettings::IniFormat);
    s.beginGroup("database");
    QString host = s.value("host").toString();
    int port = s.value("port", 5432).toInt();
    QString name = s.value("name").toString();
    QString user = s.value("user").toString();
    QString pass = s.value("password").toString();
    s.endGroup();

    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL", "main");
    database.setHostName(host);
    database.setPort(port);
    database.setDatabaseName(name);
    database.setUserName(user);
    database.setPassword(pass);

    if (!database.open()) {
        if (error) *error = database.lastError().text();
        return false;
    }
    return true;
}

bool DbManager::ensureSchema(QString *error) {
    static const char *ddl[] = {
        "CREATE TABLE IF NOT EXISTS documents (id SERIAL PRIMARY KEY, path TEXT NOT NULL UNIQUE)",
        "CREATE TABLE IF NOT EXISTS words (id SERIAL PRIMARY KEY, word TEXT NOT NULL UNIQUE)",
        "CREATE TABLE IF NOT EXISTS word_document (document_id INTEGER NOT NULL REFERENCES documents(id) ON DELETE CASCADE, word_id INTEGER NOT NULL REFERENCES words(id) ON DELETE CASCADE, frequency INTEGER NOT NULL, PRIMARY KEY (document_id, word_id))",
        "CREATE INDEX IF NOT EXISTS idx_word_document_word ON word_document(word_id)"
    };
    QSqlDatabase database = db();
    for (const char *sql : ddl) {
        QSqlQuery q(database);
        if (!q.exec(QString::fromLatin1(sql))) {
            if (error) *error = q.lastError().text();
            return false;
        }
    }
    return true;
}