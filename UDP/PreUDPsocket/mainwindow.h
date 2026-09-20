#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "udpworker.h"

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
    void on_btnSendDatagram_clicked();
    void onDatagramReceived(const QString &text,
                            const QHostAddress &senderAddress,
                            quint16 senderPort);

private:
    Ui::MainWindow *ui;
    UdpWorker *m_udpWorker = nullptr;
};

#endif