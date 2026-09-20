#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UdpWorker : public QObject
{
    Q_OBJECT
public:
    explicit UdpWorker(QObject *parent = nullptr);

    void sendDatagram(const QString &text,
                      const QHostAddress &address = QHostAddress::LocalHost,
                      quint16 port = 12345);

    bool bind(quint16 port = 12345);

signals:
    void datagramReceived(const QString &text,
                          const QHostAddress &senderAddress,
                          quint16 senderPort);

private slots:
    void onReadyRead();

private:
    QUdpSocket *m_socket = nullptr;
};

#endif