#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);

    // Подключение сигналов клиента к слотам окна
    connect(client, &TCPclient::sig_sendTime, this, &MainWindow::DisplayTime);
    connect(client, &TCPclient::sig_sendStat, this, &MainWindow::DisplayStat);
    connect(client, &TCPclient::sig_connectStatus, this, &MainWindow::DisplayConnectionStatus);
    connect(client, &TCPclient::sig_Disconnected, this, &MainWindow::DisplayDisconnected);

    // Подключение кнопки "Подключиться"
    connect(ui->pb_connect, &QPushButton::clicked, this, &MainWindow::on_pb_connect_clicked);

    // Кнопка "Отправить" (pb_request) подключена через auto-connect (on_pb_request_clicked)
    // Кнопка "Очистить" (pb_clear) уже подключена в .ui файле к tb_result->clear()
}

MainWindow::~MainWindow()
{
    delete ui;
}

// === Реализация методов по заданию ===

void MainWindow::DisplayTime(const QDateTime &time)
{
    // У нас нет lbl_time, поэтому выводим в tb_result (лог)
    ui->tb_result->append("Получено время от сервера: " + time.toString("dd.MM.yyyy hh:mm:ss"));
}

void MainWindow::DisplayStat(const StatStruct &stat)
{
    // У нас нет lbl_stat, поэтому выводим в tb_result (лог)
    QString statText = QString("Статистика сервера -> Клиентов: %1, Статус: %2")
                           .arg(stat.clientCount)
                           .arg(stat.serverStatus);

    ui->tb_result->append(statText);
}

void MainWindow::on_pb_request_clicked()
{
    ui->tb_result->append("Отправка запроса на сервер...");

    // Можно также проверить, что выбрано в cb_request, и отправить разные команды,
    // но по заданию достаточно вызвать SendRequest.
    client->SendRequest();
}

// === Вспомогательные методы ===

void MainWindow::on_pb_connect_clicked()
{
    // Собираем IP из четырех спинбоксов
    QString host = QString("%1.%2.%3.%4")
                       .arg(ui->spB_ip1->value())
                       .arg(ui->spB_ip2->value())
                       .arg(ui->spB_ip3->value())
                       .arg(ui->spB_ip4->value());

    int port = ui->spB_port->value();

    ui->tb_result->append(QString("Попытка подключения к %1:%2...").arg(host).arg(port));
    client->ConnectToHost(host, port);
}
void MainWindow::on_pb_disconnect_clicked()
{
    ui->tb_result->append("Отключение от сервера...");
    client->DisconnectFromHost();
}

void MainWindow::DisplayConnectionStatus(bool status)
{
    if (status) {
        ui->lb_connectStatus->setText("Подключено");
        ui->tb_result->append("Успешно подключено к серверу.");
    } else {
        ui->lb_connectStatus->setText("Не подключено");
        ui->tb_result->append("Соединение не установлено.");
    }
}

void MainWindow::DisplayDisconnected()
{
    ui->lb_connectStatus->setText("Отключено");
    ui->tb_result->append("Соединение с сервером разорвано.");
}