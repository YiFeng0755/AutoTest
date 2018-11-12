#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T10:24:53
#
#-------------------------------------------------

QT       += core
QT += gui
QT += widgets

TARGET = roboter
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle

win32{
  Debug:LIBS += -l$$PWD/../yim/lib/x86/yim_debug  -l$$PWD/../yim/lib/x86/YouMeCommon_debug
  Release:LIBS += -l$$PWD/../yim/lib/x86/yim  -l$$PWD/../yim/lib/YouMeCommon
}

INCLUDEPATH += $$PWD/../yim/include
DEPENDPATH += $$PWD/../yim/include

DESTDIR += $$PWD/../bin

SOURCES += main.cpp \
    mainwidget.cpp

HEADERS  += \
    mainwidget.h

FORMS    +=
