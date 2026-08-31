#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->radioButton_1->setText("Опция 1");
    ui->radioButton_2->setText("Опция 2");
    ui->radioButton_1->setChecked(true);

    ui->comboBox->addItem("Пункт 1");
    ui->comboBox->addItem("Пункт 2");
    ui->comboBox->addItem("Пункт 3");
    ui->comboBox->addItem("Пункт 4");
    ui->comboBox->addItem("Пункт 5");
    ui->comboBox->addItem("Пункт 6");

    ui->pushButton->setText("Нажми меня");
    ui->pushButton->setCheckable(true);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int currentValue = ui->progressBar->value();

    currentValue += 10;

    if (currentValue > 100) {
        currentValue = 0;
    }

    ui->progressBar->setValue(currentValue);

    qDebug() << "Прогресс:" << currentValue << "%";
}