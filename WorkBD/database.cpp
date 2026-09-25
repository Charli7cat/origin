#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase   = new QSqlDatabase();
    queryModel = new QSqlQueryModel(this);
    tableModel = new QSqlTableModel(this);
}

DataBase::~DataBase()
{
    delete dataBase;
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::ConnectToDataBase(QVector<QString> data)
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool status = dataBase->open();
    emit sig_SendStatusConnection(status);
}

void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}

void DataBase::RequestToDB(QString request)
{
    if (!dataBase->isOpen()) {
        emit sig_SendStatusRequest(
            QSqlError("База данных не открыта", "", QSqlError::ConnectionError));
        return;
    }

    queryModel->setQuery(request, *dataBase);

    if (queryModel->lastError().isValid())
        emit sig_SendStatusRequest(queryModel->lastError());
}


bool DataBase::OpenTable(const QString &tableName)
{
    if (!dataBase->isOpen()) return false;

    tableModel->setTable(tableName);
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setSort(0, Qt::AscendingOrder);

    if (!tableModel->select()) {
        emit sig_SendStatusRequest(tableModel->lastError());
        return false;
    }

    for (int i = 0; i < tableModel->columnCount(); ++i)
        tableModel->setHeaderData(i, Qt::Horizontal,
                                  tableModel->record().fieldName(i));
    return true;
}

bool DataBase::SubmitAll()
{
    if (!tableModel->submitAll()) {
        emit sig_SendStatusRequest(tableModel->lastError());
        return false;
    }
    return true;
}

void DataBase::RevertAll()
{
    tableModel->revertAll();
}

bool DataBase::AddRow()
{
    if (!tableModel) return false;
    tableModel->insertRow(tableModel->rowCount());
    return true;
}

bool DataBase::RemoveRow(int row)
{
    if (!tableModel || row < 0 || row >= tableModel->rowCount())
        return false;
    tableModel->removeRow(row);
    return true;
}

QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}