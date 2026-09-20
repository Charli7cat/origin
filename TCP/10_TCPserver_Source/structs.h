#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QDateTime>

// === Структуры ===
struct StatServer {
    quint64 incBytes = 0;
    quint64 sendBytes = 0;
    quint32 revPck = 0;
    quint32 sendPck = 0;
    QTime workTime;
    quint32 clients = 0;
};

struct ServiceHeader {
    quint16 id = 0;
    quint16 idData = 0;
    quint16 status = 0;
    quint32 len = 0;
};

struct TimeStruct {
    QDateTime time;
};

struct StatStruct {
    int clientCount;
    QString serverStatus;
};

// === Константы протокола ===
#define ID              0
#define ID_DATA         1

#define GET_SIZE        1
#define GET_TIME        2
#define GET_STAT        3
#define SET_DATA        4
#define CLEAR_SPACE     5

#define STATUS_SUCCESS  0
#define STATUS_ERROR    1

#define ERR_NO_FREE_SPACE  100
#define ERR_ZERO_LEN       101
#define ERR_UNKNOWN_CMD    102

#endif // STRUCTS_H