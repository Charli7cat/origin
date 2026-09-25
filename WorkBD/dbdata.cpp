#include "dbdata.h"
#include "ui_dbdata.h"

DbData::DbData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DbData)
{
    ui->setupUi(this);
}

DbData::~DbData()
{
    delete ui;
}

QVector<QString> DbData::connectionData() const
{
    QVector<QString> data;
    data << ui->le_host->text()
         << ui->le_dbName->text()
         << ui->le_login->text()
         << ui->le_pass->text()
         << QString::number(ui->spB_port->value());
    return data;
}