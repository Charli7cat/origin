QT       += core gui widgets sql

CONFIG   += c++17

TEMPLATE = app
TARGET   = searcher

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    searchservice.cpp \
    dbmanager.cpp

HEADERS += \
    mainwindow.h \
    searchservice.h \
    dbmanager.h

FORMS += \
    mainwindow.ui
    win32 {
        QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$$shell_path($$PWD/config.ini)\" \"$$shell_path($$OUT_PWD/debug)\")
    }