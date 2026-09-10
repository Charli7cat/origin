#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

    // --- Потоки на QThread ---
    race1 = new Controller(&m);
    race2 = new Controller(&m);

    // --- Экземпляры для QtConcurrent ---
    concurRace1 = new ExampleRace(&m);
    concurRace2 = new ExampleRace(&m);

    // Завершение первого QThread-потока
    connect(race1, &Controller::sig_WorkFinish, this, [this]() {
        if (countFinish == 0) {
            countFinish++;
        } else {
            ui->te_debug->append(
                "Искомое число равно: " + QString::number(number) +
                ", а должно быть " +
                QString::number(ui->sb_initNum->value() * 2));
            ui->pb_start->setEnabled(true);
        }
    });

    // Завершение второго QThread-потока
    connect(race2, &Controller::sig_WorkFinish, this, [this]() {
        if (countFinish == 0) {
            countFinish++;
        } else {
            ui->te_debug->append(
                "Искомое число равно: " + QString::number(number) +
                ", а должно быть " +
                QString::number(ui->sb_initNum->value() * 2));
            ui->pb_start->setEnabled(true);
        }
    });

    // Завершение первой QtConcurrent-задачи
    connect(concurRace1, &ExampleRace::sig_Finish, this, [this]() {
        if (countFinish == 0) {
            countFinish++;
        } else {
            ui->te_debug->append(
                "Искомое число равно: " + QString::number(number) +
                ", а должно быть " +
                QString::number(ui->sb_initNum->value() * 2));
            ui->pb_start->setEnabled(true);
        }
    });

    // Завершение второй QtConcurrent-задачи
    connect(concurRace2, &ExampleRace::sig_Finish, this, [this]() {
        if (countFinish == 0) {
            countFinish++;
        } else {
            ui->te_debug->append(
                "Искомое число равно: " + QString::number(number) +
                ", а должно быть " +
                QString::number(ui->sb_initNum->value() * 2));
            ui->pb_start->setEnabled(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete race1;
    delete race2;
    delete concurRace1;
    delete concurRace2;
    delete ui;
}

// Запуск потоков: либо QThread, либо QtConcurrent — в зависимости от радиокнопки
void MainWindow::StartRace()
{
    const bool mutexOn = ui->rb_mutexOn->isChecked();
    const int  numIter = ui->sb_initNum->value();

    if (ui->rb_qtConcur->isChecked()) {
        ui->te_debug->append("QtConcurrent запущен");

        // ПОСЛЕДОВАТЕЛЬНЫЙ вызов DoWork через QtConcurrent:
        // одна задача в пуле потоков выполняет сначала concurRace1, затем concurRace2.
        QFuture<void> future = QtConcurrent::run([this, mutexOn, numIter]() {
            concurRace1->DoWork(&number, mutexOn, numIter);
            concurRace2->DoWork(&number, mutexOn, numIter);
        });
        Q_UNUSED(future);   // подавляем предупреждение о неиспользуемой переменной

    } else {
        race1->operate(&number, mutexOn, numIter);
        race2->operate(&number, mutexOn, numIter);
    }
}

// Кнопка "Старт"
void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    countFinish = 0;
    number = 0;
    StartRace();
}