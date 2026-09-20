#include "dbdata.h"
#include "ui_dbdata.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>

DbData::DbData(const QString &host,
               const QString &dbName,
               const QString &user,
               const QString &password,
               int port,
               QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DbData)
{
    ui->setupUi(this);

    // Подключение к PostgreSQL (драйвер QPSQL из Qt Sql)
    m_db = QSqlDatabase::addDatabase("QPSQL", "main_connection");
    m_db.setHostName(host);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        m_connected = false;
        QMessageBox::critical(this, "Ошибка БД", m_lastError);
        return;
    }
    m_connected = true;

    // Модель для таблицы (пока без конкретной таблицы)
    m_model = new QSqlTableModel(this, m_db);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

DbData::~DbData()
{
    if (m_db.isOpen())
        m_db.close();
    delete ui;
}

bool DbData::openTable(const QString &tableName)
{
    if (!m_connected) return false;

    m_model->setTable(tableName);
    m_model->setSort(0, Qt::AscendingOrder);
    if (!m_model->select()) {
        m_lastError = m_model->lastError().text();
        return false;
    }

    // Заголовки столбцов — из описания таблицы
    for (int i = 0; i < m_model->columnCount(); ++i)
        m_model->setHeaderData(i, Qt::Horizontal,
                               m_model->record().fieldName(i));
    return true;
}

bool DbData::submitAll()
{
    if (!m_model) return false;
    if (!m_model->submitAll()) {
        m_lastError = m_model->lastError().text();
        return false;
    }
    return true;
}

void DbData::revertAll()
{
    if (m_model) m_model->revertAll();
}

bool DbData::addRow()
{
    if (!m_model) return false;
    int row = m_model->rowCount();
    m_model->insertRow(row);
    return true;
}

bool DbData::removeRow(int row)
{
    if (!m_model || row < 0 || row >= m_model->rowCount())
        return false;
    m_model->removeRow(row);
    return true;
}