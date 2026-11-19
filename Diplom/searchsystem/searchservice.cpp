#include "searchservice.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSet>

static QStringList normalizeQuery(const QStringList &raw) {
    QStringList out;
    QSet<QString> seen;
    for (QString w : raw) {
        w = w.toLower().trimmed();
        while (!w.isEmpty() && !w.front().isLetterOrNumber()) w.remove(0,1);
        while (!w.isEmpty() && !w.back().isLetterOrNumber()) w.chop(1);
        if (w.size() < 3 || seen.contains(w)) continue;
        seen.insert(w);
        out << w;
        if (out.size() == 4) break;
    }
    return out;
}

bool SearchService::search(const QStringList &queryWords, QVector<SearchHit> *hits, QString *error) {
    hits->clear();
    QStringList words = normalizeQuery(queryWords);
    if (words.isEmpty()) return true;

    QStringList placeholders;
    for (int i = 0; i < words.size(); ++i) placeholders << QString(":w%1").arg(i);

    QString sql = "SELECT d.path, SUM(wd.frequency) AS relevance "
                  "FROM documents d "
                  "JOIN word_document wd ON wd.document_id = d.id "
                  "JOIN words w ON w.id = wd.word_id "
                  "WHERE w.word IN (" + placeholders.join(", ") + ") "
                                              "GROUP BY d.id, d.path "
                                              "HAVING COUNT(DISTINCT w.word) = :cnt "
                                              "ORDER BY relevance DESC LIMIT 10";

    QSqlQuery q(DbManager::db());
    if (!q.prepare(sql)) { if (error) *error = q.lastError().text(); return false; }
    for (int i = 0; i < words.size(); ++i) q.bindValue(placeholders[i], words[i]);
    q.bindValue(":cnt", words.size());

    if (!q.exec()) { if (error) *error = q.lastError().text(); return false; }
    while (q.next()) {
        SearchHit h;
        h.path = q.value(0).toString();
        h.relevance = q.value(1).toLongLong();
        hits->append(h);
    }
    return true;
}

bool SearchService::allWordsWithFrequency(QVector<WordStat> *stats, QString *error) {
    stats->clear();
    QSqlQuery q(DbManager::db());
    if (!q.exec("SELECT w.word, COALESCE(SUM(wd.frequency),0) AS total "
                "FROM words w LEFT JOIN word_document wd ON wd.word_id = w.id "
                "GROUP BY w.word ORDER BY total DESC")) {
        if (error) *error = q.lastError().text();
        return false;
    }
    while (q.next()) stats->append({ q.value(0).toString(), q.value(1).toLongLong() });
    return true;
}