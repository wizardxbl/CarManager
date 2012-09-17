#-------------------------------------------------
#
# Project created by QtCreator 2012-09-15T15:30:41
#
#-------------------------------------------------

QT       += core gui

TARGET = CarManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialbase.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialbase.h

FORMS    += mainwindow.ui

RC_FILE += CarManager.rc
