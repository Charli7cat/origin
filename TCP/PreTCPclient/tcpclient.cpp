#include "tcpclient.h"

QDataStream& operator >>(QDataStream &in, ServiceHeader &data)
{
    in >> data.id;
    in >> data.idData;
    in >> data.status;
    in >> data.len;
    return in;
}

QDataStream& operator <<(QDataStream &out, ServiceHeader &data)
{
    out << data.id;
    out << data.idData;
    out << data.status;
    out << data.len;
    return out;
}

QDataStream& operator >>(QDataStream &in, StatServer &data)
{
    in >> data.incBytes;
    in >> data.sendBytes;
    in >> data.revPck;
    in >> data.sendPck;
    in >> data.workTime;
    in >> data.clients;
    return in;
}

QDataStream& operator <<(QDataStream &out, StatServer &data)
{
    out << data.incBytes;
    out << data.sendBytes;
    out << data.revPck;
    out << data.sendPck;
    out << data.workTime;
    out << data.clients;
    return out;
}

TCPclient::TCPclient(QObject *parent) : QObject(parent)
{
    qDebug() << ">>> TCPclient создан";

    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << ">>> Сокет ПОДКЛЮЧИЛСЯ";
        emit sig_connectStatus(STATUS_SUCCES);
    });

    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        qDebug() << ">>> Сокет ОТКЛЮЧИЛСЯ";
        emit sig_Disconnected();
    });

    connect(socket, &QTcpSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError e) {
                qDebug() << ">>> ОШИБКА СОКЕТА:" << e << socket->errorString();
                emit sig_Error(ERR_CONNECT_TO_HOST);
            });

    connect(socket, &QTcpSocket::readyRead,
            this, &TCPclient::ReadyRead);
}

void TCPclient::SendRequest(ServiceHeader head)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << head;

    socket->write(arr);
}

void TCPclient::SendData(ServiceHeader head, QString str)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
        return;

    QByteArray payload = str.toUtf8();
    head.len = uint32_t(payload.size());

    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << head;
    out.writeRawData(payload.constData(), payload.size());

    socket->write(arr);
}

void TCPclient::ConnectToHost(QHostAddress host, uint16_t port)
{
    qDebug() << ">>> ConnectToHost вызван:" << host.toString() << port;

    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->abort();

    socket->connectToHost(host, quint16(port));
}

void TCPclient::DisconnectFromHost()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        socket->disconnectFromHost();
}

void TCPclient::ReadyRead()
{
    buffer.append(socket->readAll());

    while (buffer.size() >= HEADER_SIZE) {

        QByteArray headerBytes = buffer.left(HEADER_SIZE);
        QDataStream in(&headerBytes, QIODevice::ReadOnly);
        in.setByteOrder(QDataStream::BigEndian);
        in >> servHeader;

        if (servHeader.id != ID) {
            buffer.clear();
            return;
        }

        if (buffer.size() < HEADER_SIZE + int(servHeader.len))
            return;

        QByteArray payload = buffer.mid(HEADER_SIZE, int(servHeader.len));
        buffer.remove(0, HEADER_SIZE + int(servHeader.len));

        QDataStream stream(&payload, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        ProcessingData(servHeader, stream);
    }
}

void TCPclient::ProcessingData(ServiceHeader header, QDataStream &stream)
{
    switch (header.idData) {

    case GET_TIME: {
        QDateTime time;
        stream >> time;
        emit sig_sendTime(time);
        break;
    }

    case GET_STAT: {
        StatServer stat;
        stream >> stat;
        emit sig_sendStat(stat);
        break;
    }

    case GET_SIZE: {
        uint32_t size = 0;
        stream >> size;
        emit sig_sendFreeSize(size);
        break;
    }

    case SET_DATA: {
        QString reply;
        stream >> reply;
        emit sig_SendReplyForSetData(reply);
        break;
    }

    case CLEAR_DATA: {
        emit sig_Success(CLEAR_DATA);
        break;
    }

    default:
        emit sig_Error(ERR_NO_FUNCT);
        break;
    }
}