#ifndef DBDATA_H
#define DBDATA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>

namespace Ui { class DbData; }

class DbData : public QWidget
{
    Q_OBJECT
public:
    explicit DbData(const QString &host,
                    const QString &dbName,
                    const QString &user,
                    const QString &password,
                    int port = 5432,
                    QWidget *parent = nullptr);
    ~DbData();

    bool isConnected() const { return m_connected; }
    QString lastError() const { return m_lastError; }

    QSqlTableModel *model() const { return m_model; }

    bool openTable(const QString &tableName);
    bool submitAll();
    void revertAll();
    bool addRow();
    bool removeRow(int row);

private:
    Ui::DbData *ui;
    QSqlDatabase    m_db;
    QSqlTableModel *m_model = nullptr;
    bool    m_connected = false;
    QString m_lastError;
};

#endif