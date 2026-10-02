#include "databasemanager.h"
#include <QDate>
#include <QDateTime>
#include <QLocale>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() {
    disconnectFromDb();
}

bool DatabaseManager::connectToDb()
{
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    if (m_db.isOpen()) return true;

    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("981757-ca08998.tmweb.ru");
    m_db.setPort(5432);
    m_db.setDatabaseName("demo");
    m_db.setUserName("netology_usr_cpp");
    m_db.setPassword("CppNeto3");

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "Connection error:" << m_lastError;  // ← полезно для отладки
        return false;
    }
    return true;
}

bool DatabaseManager::isConnected() const {
    return m_db.isOpen();
}

void DatabaseManager::disconnectFromDb() {
    if (m_db.isOpen()) m_db.close();
}

QMap<QString, QString> DatabaseManager::getAirports() {
    QMap<QString, QString> result;
    QSqlQuery query(m_db);
    if (query.exec("SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data")) {
        while (query.next()) {
            result.insert(query.value("airport_code").toString(),
                          query.value("airportName").toString());
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return result;
}

QVector<FlightInfo> DatabaseManager::getArrivals(const QString &airportCode, const QDate &date) {
    QVector<FlightInfo> result;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as \"Name\" "
        "FROM bookings.flights f "
        "JOIN bookings.airports_data ad ON ad.airport_code = f.departure_airport "
        "WHERE f.arrival_airport = :code "
        "AND f.scheduled_arrival::date = :date"
        );
    query.bindValue(":code", airportCode);
    query.bindValue(":date", date);

    if (query.exec()) {
        while (query.next()) {
            FlightInfo info;
            info.flightNo = query.value("flight_no").toString();
            info.time = query.value("scheduled_arrival").toDateTime();
            info.airportName = query.value("Name").toString();
            result.append(info);
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return result;
}

QVector<FlightInfo> DatabaseManager::getDepartures(const QString &airportCode, const QDate &date) {
    QVector<FlightInfo> result;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as \"Name\" "
        "FROM bookings.flights f "
        "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
        "WHERE f.departure_airport = :code "
        "AND f.scheduled_departure::date = :date"
        );
    query.bindValue(":code", airportCode);
    query.bindValue(":date", date);

    if (query.exec()) {
        while (query.next()) {
            FlightInfo info;
            info.flightNo = query.value("flight_no").toString();
            info.time = query.value("scheduled_departure").toDateTime();
            info.airportName = query.value("Name").toString();
            result.append(info);
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return result;
}

QMap<int, int> DatabaseManager::getYearStats(const QString &airportCode) {
    QMap<int, int> result;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" "
        "FROM bookings.flights f "
        "WHERE (scheduled_departure::date > date('2016-08-31') AND "
        "scheduled_departure::date <= date('2017-08-31')) "
        "AND (departure_airport = :code OR arrival_airport = :code) "
        "GROUP BY \"Month\" ORDER BY \"Month\""
        );
    query.bindValue(":code", airportCode);

    if (query.exec()) {
        while (query.next()) {
            QDateTime dt = query.value("Month").toDateTime();
            if (!dt.isValid()) continue;
            result.insert(dt.date().month(), query.value(0).toInt());
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return result;
}

QMap<int, int> DatabaseManager::getMonthStats(const QString &airportCode, int month) {
    QMap<int, int> result;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" "
        "FROM bookings.flights f "
        "WHERE (scheduled_departure::date > date('2016-08-31') AND "
        "scheduled_departure::date <= date('2017-08-31')) "
        "AND (departure_airport = :code OR arrival_airport = :code) "
        "AND EXTRACT(MONTH FROM scheduled_departure) = :month "
        "GROUP BY \"Day\" ORDER BY \"Day\""
        );
    query.bindValue(":code", airportCode);
    query.bindValue(":month", month);

    if (query.exec()) {
        while (query.next()) {
            QDateTime dt = query.value("Day").toDateTime();
            if (!dt.isValid()) continue;   // защита от NULL
            result.insert(dt.date().day(), query.value(0).toInt());
        }
    } else {
        m_lastError = query.lastError().text();
    }
    return result;
}