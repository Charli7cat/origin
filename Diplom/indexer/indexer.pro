QT       += core sql
QT       -= gui

CONFIG   += c++17 console
CONFIG   -= app_bundle

TEMPLATE = app
TARGET = indexer

SOURCES += \
    main.cpp \
    indexer.cpp \
    textparser.cpp \
    ../searchsystem/dbmanager.cpp

HEADERS += \
    indexer.h \
    textparser.h \
    ../searchsystem/dbmanager.h

INCLUDEPATH += ../searchsystem

DISTFILES += \
    config.ini
    win32 {
        QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$$shell_path($$PWD/config.ini)\" \"$$shell_path($$OUT_PWD/debug)\")
    }