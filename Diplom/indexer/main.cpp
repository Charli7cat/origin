#include "indexer.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);
    out.setEncoding(QStringConverter::System);

    QString iniPath = QCoreApplication::applicationDirPath() + "/config.ini";

    if (!QFileInfo::exists(iniPath)) {
        out << "ОШИБКА: Файл config.ini не найден рядом с исполняемым файлом.\n";
        out << "Ожидаемый путь: " << iniPath << "\n";
        return 1;
    }

    Indexer indexer;
    QObject::connect(&indexer, &Indexer::progress, [&](const QString &m){ out << m << "\n"; });

    QString error;
    if (!indexer.run(iniPath, &error)) {
        out << "КРИТИЧЕСКАЯ ОШИБКА: " << error << "\n";
        return 2;
    }
    return 0;
}