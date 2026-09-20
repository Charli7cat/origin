#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QDateTime>

struct StatServer {
    int clientCount = 0;
    QString serverStatus = "OK";
};

struct ServiceHeader {
    uint16_t type = 0;
    uint16_t size = 0;
};

struct TimeStruct {
    QDateTime time;
};

struct StatStruct {
    int clientCount;
    QString serverStatus;
};

#endif // STRUCTS_H