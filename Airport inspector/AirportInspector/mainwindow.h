#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include "databasemanager.h"
#include "loadwindow.h"

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
    void tryConnect();
    void onGetFlightsClicked();
    void onShowLoadClicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager *m_db;
    QTimer *m_reconnectTimer;
    QMap<QString, QString> m_airports;

    void updateConnectionStatus(bool connected);
    void populateAirports();
    void fillTable(const QVector<FlightInfo> &flights, bool isArrival);
};

#endif