#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T19:12:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Machine
TEMPLATE = app

DESTDIR = $$PWD/bin

SOURCES += main.cpp \
    mainwidget.cpp \
    tabwidget.cpp \
    outputwidget.cpp \
    imwrapper.cpp

HEADERS  += \
    mainwidget.h \
    tabwidget.h \
    outputwidget.h \
    imwrapper.h \
    vediowarapper.h

#link yim.lib

#LIBS += $$PWD/lib/yim/yim.dll
#LIBS += $$PWD/lib/yim/msc.dll

INCLUDEPATH += $$PWD/include/yim
DEPENDPATH += $$PWD/include/yim


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/yim/ -lyim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/yim/ -lyimd
else:unix: LIBS += -L$$PWD/lib/yim/ -lyim

INCLUDEPATH += $$PWD/include/yim
DEPENDPATH += $$PWD/include/yim

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/libyim.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/libyimd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/yim.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/yimd.lib
else:unix: PRE_TARGETDEPS += $$PWD/lib/yim/libyim.a


FORMS += \
    tabwidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/yim/ -lYouMeCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/yim/ -lYouMeCommond
else:unix: LIBS += -L$$PWD/lib/yim/ -lYouMeCommon

INCLUDEPATH += $$PWD/include/YouMeCommon
DEPENDPATH += $$PWD/include/YouMeCommon

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/libYouMeCommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/libYouMeCommond.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/YouMeCommon.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/yim/YouMeCommond.lib
else:unix: PRE_TARGETDEPS += $$PWD/lib/yim/libYouMeCommon.a
