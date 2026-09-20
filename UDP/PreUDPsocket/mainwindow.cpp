#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_udpWorker = new UdpWorker(this);

    connect(m_udpWorker, &UdpWorker::datagramReceived,
            this, &MainWindow::onDatagramReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSendDatagram_clicked()
{
    const QString text = ui->lineEditMessage->text();
    if (text.isEmpty()) return;

    m_udpWorker->sendDatagram(text,
                              QHostAddress::LocalHost,
                              12345);
}

void MainWindow::onDatagramReceived(const QString &text,
                                    const QHostAddress &senderAddress,
                                    quint16 senderPort)
{
    const int sizeBytes = text.toUtf8().size();

    QString message = QString(
                          "Принято сообщение от %1, размер сообщения(байт) %2")
                          .arg(senderAddress.toString())
                          .arg(sizeBytes);

    ui->textEditReceived->append(message);
}