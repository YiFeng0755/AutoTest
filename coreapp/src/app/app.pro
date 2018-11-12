QT += core
QT += gui

QT += widgets

QT += network
QT += sql

DESTDIR = ../../bin

SUBDIRS = \
    util

TARGET = caller
TEMPLATE = app

SOURCES += main.cpp  \
    hostsmanager.cpp \
    hostmanagerwidget.cpp \
    mainwidget.cpp \
    outputwindow.cpp \
    posthttprequest.cpp \
    cryptwidget.cpp \
    cryptutil.cpp \
    sqltest.cpp \
    globalsetting.cpp \
    requestwidget.cpp \
    digyoumerequestconfig.cpp \
    tabwidgets.cpp \
    testdemo.cpp \
    database.cpp

HEADERS += \
    posthttprequest.h \
    hostsmanager.h \
    hostmanagerwidget.h \
    mainwidget.h \
    outputwindow.h \
    wordsgenerator.h \
    cryptwidget.h \
    cryptutil.h \
    sqltest.h \
    globalsetting.h \
    requestwidget.h \
    digyoumerequestconfig.h \
    tabwidgets.h \
    testdemo.h \
    database.h

RESOURCES += \
    resource.qrc

#DISTFILES += \
#    ../../rc/youmecaller.rc

FORMS += \
    dialogforconfig.ui \
    testdemo.ui




