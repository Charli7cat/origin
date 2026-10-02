QT += core gui sql printsupport
QMAKE_CXXFLAGS += -Wa,-mbig-obj

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    databasemanager.cpp \
    loadwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    databasemanager.h \
    loadwindow.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui \
    loadwindow.ui