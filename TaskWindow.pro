#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T20:20:34
#
#-------------------------------------------------

QT       += core gui
QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskWindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyGlobalShortcut/MyGlobalShortCut.cpp \
    MyGlobalShortcut/MyWinEventFilter.cpp \
    taskwindow.cpp

HEADERS  += mainwindow.h \
    MyGlobalShortcut/MyGlobalShortCut.h \
    MyGlobalShortcut/MyWinEventFilter.h \
    taskwindow.h

FORMS    += mainwindow.ui
