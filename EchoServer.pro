#-------------------------------------------------
#
# Project created by QtCreator 2016-12-20T14:09:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EchoServer
TEMPLATE = app


SOURCES += main.cpp\
        mainview.cpp

HEADERS  += mainview.h

FORMS    += mainview.ui

QMAKE_CXXFLAGS += -std=c++11
