#include "mainwindow.h"
#include "dbmanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QFileInfo>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString iniPath = QCoreApplication::applicationDirPath() + "/config.ini";
    if (!QFileInfo::exists(iniPath)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не найден config.ini рядом с exe");
        return 1;
    }

    QString err;
    if (!DbManager::connect(iniPath, &err)) {
        QMessageBox::critical(nullptr, "Ошибка БД", err);
        return 2;
    }

    MainWindow w(iniPath);
    w.show();
    return app.exec();
}