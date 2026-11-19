#include "textparser.h"
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

bool TextParser::parseFile(const QString &path,
                           const ParserConfig &cfg,
                           QHash<QString,int> *freq,
                           QString *error) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (error) *error = QString("Не могу открыть %1: %2").arg(path, f.errorString());
        return false;
    }
    QTextStream in(&f);
    static const QRegularExpression re(QStringLiteral("[^\\p{L}\\p{N}]+"));

    freq->clear();
    while (!in.atEnd()) {
        const QString line = in.readLine();
        const QStringList tokens = line.split(re, Qt::SkipEmptyParts);
        for (QString w : tokens) {
            w = w.toLower();
            const int n = w.size();
            if (n < cfg.minLen || n > cfg.maxLen) continue;
            ++(*freq)[w];
        }
    }
    f.close();
    return true;
}