#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectionDialog; }
QT_END_NAMESPACE

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

    QString getHost() const;
    QString getDatabaseName() const;
    QString getLogin() const;
    QString getPassword() const;
    uint getPort() const;

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ConnectionDialog *ui;
};

#endif
