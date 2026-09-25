#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlError>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();
    void on_btnClear_clicked();
    void on_act_connect_triggered();
    void on_act_addData_triggered();

    void onStatusConnection(bool status);
    void onStatusRequest(QSqlError err);

private:
    Ui::MainWindow *ui;
    DataBase *db = nullptr;
};

#endif