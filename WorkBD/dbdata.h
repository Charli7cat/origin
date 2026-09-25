#ifndef DBDATA_H
#define DBDATA_H

#include <QDialog>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class DbData; }
QT_END_NAMESPACE

class DbData : public QDialog
{
    Q_OBJECT

public:
    explicit DbData(QWidget *parent = nullptr);
    ~DbData();

    QVector<QString> connectionData() const;

private:
    Ui::DbData *ui;
};

#endif