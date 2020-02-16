#-------------------------------------------------
#
# Project created by QtCreator 2013-06-23T14:13:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tetris
TEMPLATE = app


SOURCES += main.cpp\
        tetris.cpp \
    tetrisbox.cpp

HEADERS  += tetris.h \
    tetrisbox.h

FORMS    += tetris.ui

RESOURCES += \
    picture.qrc
