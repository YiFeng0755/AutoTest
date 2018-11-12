#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T10:24:53
#
#-------------------------------------------------

QT += core
QT += gui
QT += widgets
QT += network
QT += concurrent
QT += network
QT += sql

TARGET = IMDemo
TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
LIBS += -lDbgHelp
win32{
  Debug:LIBS += -l$$PWD/../yim/lib/x86/yim_debug  -l$$PWD/../yim/lib/x86/YouMeCommon_debug
  Release:LIBS += -l$$PWD/../yim/lib/x86/yim  -l$$PWD/../yim/lib/YouMeCommon
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += $$PWD/../yim/include
DEPENDPATH += $$PWD/../yim/include

DESTDIR += $$PWD/../bin

SOURCES += main.cpp \
    mainwidget.cpp \
    outputwindow.cpp \
    functions.cpp \
    dlgyoumeappkeyconfig.cpp \
    globalsetting.cpp \
    database.cpp \
    httprequester.cpp \
    cryptutil.cpp \
    roommanager.cpp \
    dlgperftest.cpp \
    performancewidget.cpp \
    stabletestdialog.cpp

HEADERS  += \
    callbackwrapper.h \
    functions.h \
    mainwidget.h \
    outputwindow.h \
    dlgyoumeappkeyconfig.h \
    globalsetting.h \
    database.h \
    httprequester.h \
    cryptutil.h \
    roommanager.h \
    dlgperftest.h \
    performancewidget.h \
    stabletestdialog.h



FORMS    += \
    functions.ui \
    mainwidget.ui \
    dialogforconfig.ui \
    roommanager.ui \
    performancewidget.ui \
    stableform.ui

RESOURCES += \
    resources.qrc

RC_FILE += app.rc
