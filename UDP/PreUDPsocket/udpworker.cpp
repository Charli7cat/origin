#include "udpworker.h"

UdpWorker::UdpWorker(QObject *parent)
    : QObject(parent)
    , m_socket(new QUdpSocket(this))
{
    if (!m_socket->bind(QHostAddress::LocalHost, 12345)) {
        qWarning() << "Не удалось привязать сокет:"
                   << m_socket->errorString();
    }

    connect(m_socket, &QUdpSocket::readyRead,
            this, &UdpWorker::onReadyRead);
}

void UdpWorker::sendDatagram(const QString &text,
                             const QHostAddress &address,
                             quint16 port)
{
    QByteArray data = text.toUtf8();
    qint64 sent = m_socket->writeDatagram(data, address, port);
    if (sent == -1) {
        qWarning() << "Ошибка отправки:" << m_socket->errorString();
    }
}

bool UdpWorker::bind(quint16 port)
{
    return m_socket->bind(QHostAddress::LocalHost, port);
}

void UdpWorker::onReadyRead()
{
    while (m_socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(int(m_socket->pendingDatagramSize()));

        QHostAddress senderAddress;
        quint16 senderPort = 0;

        m_socket->readDatagram(buffer.data(), buffer.size(),
                               &senderAddress, &senderPort);

        QString text = QString::fromUtf8(buffer);
        emit datagramReceived(text, senderAddress, senderPort);
    }
}