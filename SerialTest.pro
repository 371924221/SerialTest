#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T21:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport
TARGET = SerialTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    idevicemanager.cpp \
    base.cpp \
    datafilter.cpp \
    params.cpp

HEADERS  += mainwindow.h \
    idevicemanager.h \
    base.h \
    datafilter.h \
    Params.h

FORMS    += mainwindow.ui
