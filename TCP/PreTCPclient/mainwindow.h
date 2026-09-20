#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structs.h"
#include <QMainWindow>
#include "tcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_request_clicked();
    void on_pb_connect_clicked();    // Предполагаемый слот для подключения
    void on_pb_disconnect_clicked(); // Предполагаемый слот для отключения

    // Слоты для приема сигналов от TCPclient
    void DisplayTime(const QDateTime &time);
    void DisplayStat(const StatStruct &stat);
    void DisplayConnectionStatus(bool status);
    void DisplayDisconnected();

private:
    Ui::MainWindow *ui;
    TCPclient *client;
};
#endif // MAINWINDOW_H