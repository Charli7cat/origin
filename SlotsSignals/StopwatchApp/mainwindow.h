#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Stopwatch.h"

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
    void onStartStopButtonClicked();
    void onResetButtonClicked();
    void onLapButtonClicked();
    void updateTimeDisplay(qint64 milliseconds);

private:
    Ui::MainWindow *ui;
    Stopwatch *m_stopwatch;
    int m_lapCounter;
    qint64 m_lastLapTimeMs;
    QString formatTime(qint64 milliseconds) const;
};

#endif