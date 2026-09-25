#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <QTime>
#include <cstring>

#define ID 0xFFFE
#define TYPE_REQ 10
#define TYPE_ANS 20

#pragma pack(push, 1)

enum Messages {
    GET_TIME   = 100,
    GET_SIZE   = 101,
    GET_STAT   = 102,
    SET_DATA   = 200,
    CLEAR_DATA = 201
};

enum StatusMessages {
    STATUS_SUCCES       = 1,
    ERR_NO_FREE_SPACE   = 10,
    ERR_CONNECT_TO_HOST = 11,
    ERR_NO_FUNCT        = 20
};

/*!
 * \brief Служебный заголовок пакета
 */
struct ServiceHeader {
    uint16_t id     = ID;
    uint16_t idData = 0;
    uint8_t  status = 0;
    uint32_t len    = 0;
};

/*!
 * \brief Статистика сервера
 */
struct StatServer {
    StatServer() { memset(this, 0, sizeof(*this)); }

    uint32_t incBytes;
    uint32_t sendBytes;
    uint32_t revPck;
    uint32_t sendPck;
    uint32_t workTime;
    uint32_t clients;
};

#pragma pack(pop)
#endif // STRUCTS_H