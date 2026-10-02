#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QDialog>
#include "qcustomplot.h"
#include "databasemanager.h"

namespace Ui { class LoadWindow; }

class LoadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadWindow(DatabaseManager *db, const QString &airportCode,
                        const QString &airportName, QWidget *parent = nullptr);
    ~LoadWindow();

private slots:
    void onMonthChanged(int index);
    void onCloseClicked();

private:
    Ui::LoadWindow *ui;
    DatabaseManager *m_db;
    QString m_airportCode;

    QCustomPlot *m_yearPlot = nullptr;
    QCustomPlot *m_monthPlot = nullptr;

    void buildYearChart();
    void buildMonthChart(int month);
};

#endif