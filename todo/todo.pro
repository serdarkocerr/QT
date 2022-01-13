#-------------------------------------------------
#
# Project created by QtCreator 2022-01-12T17:33:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = todo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    task.cpp

HEADERS  += mainwindow.h \
    task.h

FORMS    += mainwindow.ui \
    task.ui
