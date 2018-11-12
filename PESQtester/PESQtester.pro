#-------------------------------------------------
#
# Project created by QtCreator 2018-01-30T18:54:37
#
#-------------------------------------------------

QT       += core gui

QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PESQtester
TEMPLATE = app
DESTDIR =  $$PWD/bin

SOURCES += main.cpp\
        mainwidget.cpp \
    servernetworkcontroller.cpp \
    outputwindow.cpp \
    settings.cpp \
    dlgquecmds.cpp \
    pesqthread.cpp

HEADERS  += mainwidget.h \
    servernetworkcontroller.h \
    myglobal.h \
    outputwindow.h \
    settings.h \
    dlgquecmds.h \
    pesqthread.h

FORMS    += mainwidget.ui \
    hostsettingdialog.ui \
    settingdialog.ui \
    cmdlistwidget.ui

RC_FILE += app.rc

OBJECTS_DIR += tmp/obj
RCC_DIR += tmp/rc
MOC_DIR += tmp/moc


RESOURCES += \
    resource.qrc
