#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionDialog.h"
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton *btn = new QPushButton("Подключиться к БД", this);
    setCentralWidget(btn);
    connect(btn, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    ConnectionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString info = QString(
                           "Хост: %1\n"
                           "БД: %2\n"
                           "Логин: %3\n"
                           "Пароль: %4\n"
                           "Порт: %5"
                           ).arg(dialog.getHost())
                           .arg(dialog.getDatabaseName())
                           .arg(dialog.getLogin())
                           .arg(dialog.getPassword())
                           .arg(dialog.getPort());

        QMessageBox::information(this, "Данные подключения", info);
    }
}