#-------------------------------------------------
#
# Project created by QtCreator 2018-08-11T14:08:18
#
#-------------------------------------------------

QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatter
TEMPLATE = app

win32{
  Debug:LIBS += -l$$PWD/../yim/lib/x86/yim_debug  -l$$PWD/../yim/lib/x86/YouMeCommon_debug
  Release:LIBS += -l$$PWD/../yim/lib/x86/yim  -l$$PWD/../yim/lib/YouMeCommon
}

INCLUDEPATH += $$PWD/../yim/include
DEPENDPATH += $$PWD/../yim/include

DESTDIR += $$PWD/../bin


SOURCES += main.cpp\
    logindialog.cpp \
    chatwidget.cpp \
    ../main/httprequester.cpp \
    ../main/globalsetting.cpp \
    ../main/cryptutil.cpp \
    ../main/outputwindow.cpp

HEADERS  += \
    logindialog.h \
    chatwidget.h \
    ../main/httprequester.h \
    ../main/globalsetting.h \
    ../main/cryptutil.cpp \
    ../main/outputwindow.h

FORMS    += chatform.ui
