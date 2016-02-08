#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T22:41:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureSize
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    bicubic.h \
    bilinear.h \
    manipulate.h \
    neighbor.h \
    pixel.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc

RC_ICONS = mainIcon.ico
