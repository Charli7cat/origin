#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>

#include "ExampleRace.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pb_start_clicked();

private:
    void StartRace();

    Ui::MainWindow* ui;

    // Общий мьютекс и общая переменная для всех потоков
    QMutex m;
    uint32_t number = 0;
    int countFinish = 0;

    // Потоки на QThread
    Controller* race1 = nullptr;
    Controller* race2 = nullptr;

    // Экземпляры для QtConcurrent
    ExampleRace* concurRace1 = nullptr;
    ExampleRace* concurRace2 = nullptr;
};

#endif // MAINWINDOW_H

