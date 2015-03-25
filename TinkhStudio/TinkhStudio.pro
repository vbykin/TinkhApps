#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T09:43:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TinkhStudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pieceslist.cpp \
    puzzlewidget.cpp \
    brickdata.cpp \
    playpenscene.cpp

HEADERS  += mainwindow.h \
    pieceslist.h \
    puzzlewidget.h \
    brickdata.h \
    playpenscene.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    tinkhstudio.qrc
