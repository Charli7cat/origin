#pragma once
#include <QString>
#include <QHash>

struct ParserConfig {
    int minLen = 3;
    int maxLen = 32;
};

class TextParser {
public:
    static bool parseFile(const QString &path,
                          const ParserConfig &cfg,
                          QHash<QString,int> *freq,
                          QString *error);
};