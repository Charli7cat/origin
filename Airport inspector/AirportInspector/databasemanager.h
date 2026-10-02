#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QLocale>


struct FlightInfo {
    QString flightNo;
    QDateTime time;
    QString airportName;
};

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectToDb();
    bool isConnected() const;
    void disconnectFromDb();

    QMap<QString, QString> getAirports();
    QVector<FlightInfo> getArrivals(const QString &airportCode, const QDate &date);
    QVector<FlightInfo> getDepartures(const QString &airportCode, const QDate &date);
    QMap<int, int> getYearStats(const QString &airportCode);
    QMap<int, int> getMonthStats(const QString &airportCode, int month);

    QString lastError() const { return m_lastError; }

private:
    QSqlDatabase m_db;
    QString m_lastError;
};

#endif