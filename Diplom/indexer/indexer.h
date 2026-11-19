#pragma once
#include <QObject>
#include <QSet>
#include <QStringList>
#include <QHash>
#include "textparser.h"

class Indexer : public QObject {
    Q_OBJECT
public:
    explicit Indexer(QObject *parent = nullptr);
    bool run(const QString &iniPath, QString *error);

signals:
    void progress(const QString &msg);

private:
    void collectFiles(const QString &dir, const QStringList &exts, QSet<QString> *out);
    bool indexFile(const QString &path, const ParserConfig &cfg, QString *error);
    bool saveDocument(const QString &path, const QHash<QString,int> &freq, QString *error);
    int  upsertWord(const QString &word, QString *error);
    int  upsertDocument(const QString &path, QString *error);
};