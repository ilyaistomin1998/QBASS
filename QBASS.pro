#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T19:14:45
#
#-------------------------------------------------

QT       += core gui

TARGET = QBASS
TEMPLATE = app

QT += widgets

SOURCES += main.cpp\
        audioplayer.cpp

HEADERS  += audioplayer.h\
        bass.h

FORMS    += audioplayer.ui

unix:!macx: LIBS += -L$$PWD/./ -lbass

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
