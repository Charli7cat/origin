#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QVector>

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void ConnectToDataBase(QVector<QString> data);
    void DisconnectFromDataBase(QString nameDb);
    void RequestToDB(QString request);

    bool OpenTable(const QString &tableName);
    bool SubmitAll();
    void RevertAll();
    bool AddRow();
    bool RemoveRow(int row);

    QSqlError GetLastError();

    QSqlQueryModel* GetQueryModel() const { return queryModel; }
    QSqlTableModel* GetTableModel() const { return tableModel; }

signals:
    void sig_SendStatusConnection(bool status);
    void sig_SendStatusRequest(QSqlError err);

private:
    QSqlDatabase   *dataBase   = nullptr;
    QSqlQueryModel *queryModel = nullptr;
    QSqlTableModel *tableModel = nullptr;

    enum Columns { hostName, dbName, login, pass, port };
};

#endif