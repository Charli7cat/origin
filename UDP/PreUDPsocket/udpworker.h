#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QHostAddress>        // <-- добавить
#include <QDateTime>
#include <QByteArray>          // <-- на всякий случай

#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT

public:
    explicit UDPworker(QObject *parent = nullptr);

    void InitSocket();
    void ReadDatagram(QNetworkDatagram datagram);
    void SendDatagram(QByteArray data);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket* serviceUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendMessageToGUI(QString message);   // <-- см. Шаг 5
};

#endif // UDPWORKER_H