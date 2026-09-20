#include "tcpclient.h"
#include <QDataStream>

TCPclient::TCPclient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    // Соединяем сигналы сокета со слотами класса
    connect(socket, &QTcpSocket::readyRead, this, &TCPclient::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &TCPclient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &TCPclient::onDisconnected);
}

void TCPclient::ConnectToHost(const QString &host, int port)
{
    socket->connectToHost(host, port);
}

void TCPclient::DisconnectFromHost()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}

void TCPclient::SendRequest()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        // Отправляем запрос серверу (например, просто текстовую команду)
        socket->write("GET_DATA");
    }
}

void TCPclient::onConnected()
{
    emit sig_connectStatus(true);
}

void TCPclient::onDisconnected()
{
    emit sig_Disconnected();
    emit sig_connectStatus(false);
}

void TCPclient::onReadyRead()
{
    QByteArray data = socket->readAll();
    ProcessingData(data);
}

void TCPclient::ProcessingData(const QByteArray &data)
{
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_15);

    // Предполагаем, что сервер присылает тип пакета, затем данные
    int packetType;
    stream >> packetType;

    if (packetType == 1) { // 1 = Время
        QDateTime time;
        stream >> time;
        emit sig_sendTime(time);
    }
    else if (packetType == 2) { // 2 = Статистика
        StatStruct stat;
        stream >> stat.clientCount;
        stream >> stat.serverStatus;
        emit sig_sendStat(stat);
    }
}