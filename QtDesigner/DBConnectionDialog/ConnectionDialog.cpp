#include "ConnectionDialog.h"      // ← используется для класса ConnectionDialog
#include "ui_ConnectionDialog.h"   // ← используется для Ui::ConnectionDialog

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog)   // ← использует ui_connectiondialog.h
{
    ui->setupUi(this);               // ← использует ui_connectiondialog.h

    ui->hostLineEdit->setText("localhost");
    ui->dbNameLineEdit->setText("mydatabase");
    ui->loginLineEdit->setText("root");
    ui->passwordLineEdit->setText("");
    ui->portSpinBox->setValue(3306);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;                       // ← использует ui_connectiondialog.h
}

QString ConnectionDialog::getHost() const
{
    return ui->hostLineEdit->text(); // ← использует ui_connectiondialog.h
}

QString ConnectionDialog::getDatabaseName() const
{
    return ui->dbNameLineEdit->text();
}

QString ConnectionDialog::getLogin() const
{
    return ui->loginLineEdit->text();
}

QString ConnectionDialog::getPassword() const
{
    return ui->passwordLineEdit->text();
}

uint ConnectionDialog::getPort() const
{
    return static_cast<uint>(ui->portSpinBox->value());
}

void ConnectionDialog::on_okButton_clicked()
{
    accept();                        // ← использует QDialog из connectiondialog.h
}

void ConnectionDialog::on_cancelButton_clicked()
{
    reject();                        // ← использует QDialog из connectiondialog.h
}