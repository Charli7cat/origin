#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>

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
    void on_btnApply_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase    m_db;
    QSqlTableModel *m_tableModel = nullptr;
    QSqlQueryModel *m_queryModel = nullptr;

    void loadAllFilms();
    void loadFilmsByGenre(const QString &genre);
};

#endif