#pragma once
#include <QMainWindow>
#include <QVector>
#include "searchservice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const QString &iniPath, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();
    void onShowDocumentsClicked();

private:
    void showError(const QString &msg);
    void loadWordsTab();

    Ui::MainWindow *ui;
    QVector<SearchHit> lastHits;
    QString iniPath_;
};