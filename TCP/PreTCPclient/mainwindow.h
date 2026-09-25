#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include "tcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_connect_clicked();
    void on_pb_request_clicked();
    void on_cb_request_currentIndexChanged(int index);

    void DisplayTime(QDateTime time);
    void DisplayFreeSpace(uint32_t freeSpace);
    void SetDataReply(QString replyString);
    void DisplayStat(StatServer stat);
    void DisplayError(uint16_t error);
    void DisplaySuccess(uint16_t typeMess);
    void DisplayConnectStatus(uint16_t status);

private:
    Ui::MainWindow *ui;
    TCPclient *client;
    bool isConnected = false;
};

#endif // MAINWINDOW_H