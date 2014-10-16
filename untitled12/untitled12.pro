#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T22:00:26
#
#-------------------------------------------------

QT       += core gui
QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled12
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    taskwindow.cpp \
    MyGlobalShortcut/MyGlobalShortCut.cpp \
    MyGlobalShortcut/MyWinEventFilter.cpp

HEADERS  += mainwindow.h \
    taskwindow.h \
    MyGlobalShortcut/MyGlobalShortCut.h \
    MyGlobalShortcut/MyWinEventFilter.h

FORMS    += mainwindow.ui
LIBS += -luser32 -lshell32 -lgdi32
LIBS +=  Psapi.Lib

OTHER_FILES +=
RC_FILE += app.rc

