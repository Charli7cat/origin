#pragma once
#include <QSqlDatabase>
#include <QString>

class DbManager {
public:
    static bool connect(const QString &iniPath, QString *error = nullptr);
    static bool ensureSchema(QString *error = nullptr);
    static QSqlDatabase db();
};