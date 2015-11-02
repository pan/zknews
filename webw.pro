#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T19:34:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = webw
TEMPLATE = app


SOURCES += main.cpp\
        webwid.cpp \
    systray.cpp

HEADERS  += webwid.h \
    systray.h

FORMS    += webwid.ui

RESOURCES += \
    pic.qrc

DISTFILES +=
