#include "indexer.h"
#include "dbmanager.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

Indexer::Indexer(QObject *parent) : QObject(parent) {}

bool Indexer::run(const QString &iniPath, QString *error) {
    QSettings s(iniPath, QSettings::IniFormat);
    s.beginGroup("indexer");
    const QStringList dirs = s.value("directories").toString().split(';', Qt::SkipEmptyParts);
    QStringList rawExts = s.value("extensions").toString().split(',', Qt::SkipEmptyParts);
    const QStringList exts = s.value("extensions").toString().split(',', Qt::SkipEmptyParts);
    ParserConfig cfg;
    cfg.minLen = s.value("minWordLength", 3).toInt();
    cfg.maxLen = s.value("maxWordLength", 32).toInt();
    s.endGroup();

    if (dirs.isEmpty()) { if (error) *error = "Не указаны каталоги в config.ini"; return false; }

    if (!DbManager::connect(iniPath, error)) return false;
    if (!DbManager::ensureSchema(error)) return false;

    QSet<QString> files;
    for (const QString &d : dirs)
        collectFiles(d.trimmed(), exts, &files);

    emit progress(QString("Найдено файлов: %1").arg(files.size()));

    int ok = 0, fail = 0;
    for (const QString &path : files) {
        if (indexFile(path, cfg, error)) ++ok;
        else {
            ++fail;
            emit progress(QString("Ошибка в файле %1: %2").arg(path, error ? *error : ""));
        }
    }
    emit progress(QString("Готово. Успешно: %1, ошибок: %2").arg(ok).arg(fail));
    return true;
}

void Indexer::collectFiles(const QString &dir, const QStringList &exts, QSet<QString> *out) {
    QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString path = it.next();
        const QString suffix = QFileInfo(path).suffix().toLower();
        for (const QString &e : exts) {
            if (suffix == e.trimmed().toLower()) {
                out->insert(path);
                break;
            }
        }
    }
}

bool Indexer::indexFile(const QString &path, const ParserConfig &cfg, QString *error) {
    QHash<QString,int> freq;
    if (!TextParser::parseFile(path, cfg, &freq, error)) return false;
    if (freq.isEmpty()) return true;
    return saveDocument(path, freq, error);
}

int Indexer::upsertDocument(const QString &path, QString *error) {
    QSqlQuery q(DbManager::db());
    q.prepare("INSERT INTO documents(path) VALUES(:p) "
              "ON CONFLICT (path) DO UPDATE SET path = EXCLUDED.path "
              "RETURNING id");
    q.bindValue(":p", path);
    if (!q.exec() || !q.next()) {
        if (error) *error = q.lastError().text();
        return -1;
    }
    return q.value(0).toInt();
}

int Indexer::upsertWord(const QString &word, QString *error) {
    QSqlQuery q(DbManager::db());
    q.prepare("INSERT INTO words(word) VALUES(:w) "
              "ON CONFLICT (word) DO UPDATE SET word = EXCLUDED.word "
              "RETURNING id");
    q.bindValue(":w", word);
    if (!q.exec() || !q.next()) {
        if (error) *error = q.lastError().text();
        return -1;
    }
    return q.value(0).toInt();
}

bool Indexer::saveDocument(const QString &path, const QHash<QString,int> &freq, QString *error) {
    QSqlDatabase database = DbManager::db();
    database.transaction();

    const int docId = upsertDocument(path, error);
    if (docId < 0) { database.rollback(); return false; }

    {
        QSqlQuery del(database);
        del.prepare("DELETE FROM word_document WHERE document_id = :d");
        del.bindValue(":d", docId);
        if (!del.exec()) { if (error) *error = del.lastError().text(); database.rollback(); return false; }
    }

    QSqlQuery ins(database);
    ins.prepare("INSERT INTO word_document(document_id, word_id, frequency) "
                "VALUES(:d, :w, :f)");
    for (auto it = freq.constBegin(); it != freq.constEnd(); ++it) {
        const int wordId = upsertWord(it.key(), error);
        if (wordId < 0) { database.rollback(); return false; }
        ins.bindValue(":d", docId);
        ins.bindValue(":w", wordId);
        ins.bindValue(":f", it.value());
        if (!ins.exec()) { if (error) *error = ins.lastError().text(); database.rollback(); return false; }
    }

    database.commit();
    return true;
}