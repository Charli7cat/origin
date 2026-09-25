#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbdata.h"

#include <QMessageBox>
#include <QSqlError>
#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase(this);

    connect(db, &DataBase::sig_SendStatusConnection,
            this, &MainWindow::onStatusConnection);
    connect(db, &DataBase::sig_SendStatusRequest,
            this, &MainWindow::onStatusRequest);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    DbData dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    db->AddDataBase("QPSQL", "main_connection");
    db->ConnectToDataBase(dlg.connectionData());
}

void MainWindow::on_btnClear_clicked()
{
    ui->comboBoxFilter->setCurrentIndex(0);
    ui->tableView->clearSelection();
}

void MainWindow::on_act_connect_triggered()
{
    on_btnConnect_clicked();
}

void MainWindow::on_act_addData_triggered()
{
    db->AddRow();
}

void MainWindow::onStatusConnection(bool status)
{
    if (!status) {
        ui->lb_statusConnect->setText("Не подключено");
        QMessageBox::critical(this, "Ошибка подключения",
                              db->GetLastError().text());
        return;
    }

    ui->lb_statusConnect->setText("Подключено");

    db->OpenTable("имя_вашей_таблицы");
    ui->tableView->setModel(db->GetTableModel());
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::onStatusRequest(QSqlError err)
{
    QMessageBox::critical(this, "Ошибка БД", err.text());
}