#pragma once
#include <QStringList>
#include <QVector>

struct SearchHit { QString path; qint64 relevance = 0; };
struct WordStat  { QString word; qint64 frequency = 0; };

class SearchService {
public:
    static bool search(const QStringList &queryWords, QVector<SearchHit> *hits, QString *error);
    static bool allWordsWithFrequency(QVector<WordStat> *stats, QString *error);
};
