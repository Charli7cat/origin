#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "structs.h"

class TCPclient : public QObject
{
    Q_OBJECT
public:
    explicit TCPclient(QObject *parent = nullptr);
    void ConnectToHost(const QString &host, int port);
    void DisconnectFromHost();
    void SendRequest();

signals:
    void sig_sendTime(const QDateTime &time);
    void sig_sendStat(const StatStruct &stat);
    void sig_Disconnected();
    void sig_connectStatus(bool connected);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    QTcpSocket *socket;
    void ProcessingData(const QByteArray &data);
};

#endif // TCPCLIENT_H