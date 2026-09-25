#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);

    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");

    connect(client, &TCPclient::sig_Disconnected, this, [this]{
        isConnected = false;
        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);
    });

    connect(client, &TCPclient::sig_sendTime,
            this, &MainWindow::DisplayTime);

    connect(client, &TCPclient::sig_sendFreeSize,
            this, &MainWindow::DisplayFreeSpace);

    connect(client, &TCPclient::sig_SendReplyForSetData,
            this, &MainWindow::SetDataReply);

    connect(client, &TCPclient::sig_sendStat,
            this, &MainWindow::DisplayStat);

    connect(client, &TCPclient::sig_Error,
            this, &MainWindow::DisplayError);

    connect(client, &TCPclient::sig_Success,
            this, &MainWindow::DisplaySuccess);

    connect(client, &TCPclient::sig_connectStatus,
            this, &MainWindow::DisplayConnectStatus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayTime(QDateTime time)
{
    ui->tb_result->append("Время сервера: "
                          + time.toString("HH:mm:ss dd.MM.yyyy"));
}

void MainWindow::DisplayFreeSpace(uint32_t freeSpace)
{
    ui->tb_result->append("Свободное место на сервере: "
                          + QString::number(freeSpace) + " байт");
}

void MainWindow::SetDataReply(QString replyString)
{
    ui->tb_result->append("Ответ на отправку данных: " + replyString);
}

void MainWindow::DisplayStat(StatServer stat)
{
    ui->tb_result->append("=== Статистика сервера ===");
    ui->tb_result->append("Принято байт:      " + QString::number(stat.incBytes));
    ui->tb_result->append("Передано байт:     " + QString::number(stat.sendBytes));
    ui->tb_result->append("Принято пакетов:   " + QString::number(stat.revPck));
    ui->tb_result->append("Передано пакетов:  " + QString::number(stat.sendPck));
    ui->tb_result->append("Время работы:      " + QString::number(stat.workTime) + " сек");
    ui->tb_result->append("Клиентов:          " + QString::number(stat.clients));
    ui->tb_result->append("===========================");
}

void MainWindow::DisplayError(uint16_t error)
{
    switch (error) {
    case ERR_NO_FREE_SPACE:
        ui->tb_result->append("Ошибка: недостаточно свободного места на сервере");
        break;
    case ERR_CONNECT_TO_HOST:
        ui->tb_result->append("Ошибка: не удалось подключиться к серверу");
        break;
    case ERR_NO_FUNCT:
        ui->tb_result->append("Ошибка: такой функционал не реализован");
        break;
    default:
        ui->tb_result->append("Неизвестная ошибка: " + QString::number(error));
        break;
    }
}

void MainWindow::DisplaySuccess(uint16_t typeMess)
{
    switch (typeMess) {
    case CLEAR_DATA:
        ui->tb_result->append("Память на сервере успешно очищена");
        break;
    default:
        ui->tb_result->append("Сообщение успешно обработано, код: "
                              + QString::number(typeMess));
        break;
    }
}

void MainWindow::DisplayConnectStatus(uint16_t status)
{
    if (status == ERR_CONNECT_TO_HOST) {
        ui->tb_result->append("Ошибка подключения к порту: "
                              + QString::number(ui->spB_port->value()));
    }
    else {
        isConnected = true;
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);
    }
}

void MainWindow::on_pb_connect_clicked()
{
    qDebug() << ">>> Кнопка нажата";

    if (!isConnected) {
        uint16_t port = uint16_t(ui->spB_port->value());

        QString ip = QString::number(ui->spB_ip4->value()) + "." +
                     QString::number(ui->spB_ip3->value()) + "." +
                     QString::number(ui->spB_ip2->value()) + "." +
                     QString::number(ui->spB_ip1->value());

        qDebug() << ">>> IP:" << ip << "PORT:" << port;
        client->ConnectToHost(QHostAddress(ip), port);
    }
    else {
        client->DisconnectFromHost();
    }
}

void MainWindow::on_pb_request_clicked()
{
    ServiceHeader header;
    header.id     = ID;
    header.status = STATUS_SUCCES;
    header.len    = 0;

    switch (ui->cb_request->currentIndex()) {

    case 0:
        header.idData = GET_TIME;
        client->SendRequest(header);
        break;

    case 1:
        header.idData = GET_SIZE;
        client->SendRequest(header);
        break;

    case 2:
        header.idData = GET_STAT;
        client->SendRequest(header);
        break;

    case 3:
        header.idData = SET_DATA;
        client->SendData(header, ui->le_data->text());
        break;

    case 4:
        header.idData = CLEAR_DATA;
        client->SendRequest(header);
        break;

    default:
        ui->tb_result->append("Такой запрос не реализован в текущей версии");
        break;
    }
}

void MainWindow::on_cb_request_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (ui->cb_request->currentIndex() == 3)
        ui->le_data->setEnabled(true);
    else
        ui->le_data->setEnabled(false);
}