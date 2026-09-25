#include "udpworker.h"
#include <QDataStream>
#include <QIODevice>
#include <QHostAddress>

UDPworker::UDPworker(QObject *parent)
    : QObject(parent)
    , serviceUdpSocket(nullptr)
{
}

void UDPworker::InitSocket()
{
    serviceUdpSocket = new QUdpSocket(this);

    // Привязываемся к localhost:12345, чтобы принимать свои же датаграммы
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead,
            this, &UDPworker::readPendingDatagrams);
}

void UDPworker::readPendingDatagrams()
{
    // Пока есть непрочитанные датаграммы
    while (serviceUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
        ReadDatagram(datagram);
    }
}

void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    // По заданию выводим: адрес отправителя и размер сообщения
    QByteArray data = datagram.data();
    QString sender = datagram.senderAddress().toString()
                     + ":" + QString::number(datagram.senderPort());

    // Если хотите сохранить старую логику с QDateTime — оставьте её
    // (она полезна при кнопке "Начать передачу"), но по заданию
    // нужен вывод адреса и размера:

    QString message = QString("Принято сообщение от %1, размер сообщения %2 байт")
                          .arg(sender)
                          .arg(data.size());

    emit sig_sendMessageToGUI(message);
}

void UDPworker::SendDatagram(QByteArray data)
{
    // Отправляем на localhost:BIND_PORT
    serviceUdpSocket->writeDatagram(data,
                                    QHostAddress::LocalHost,
                                    BIND_PORT);
}