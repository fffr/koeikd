#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T09:02:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
                                    serialport

TARGET = koeikd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    candialog.cpp \
    canmessageedit.cpp \
    canrxworker.cpp \
    lcarddialog.cpp \
    lcardworker.cpp \
    video/view.cpp

HEADERS  += mainwindow.h \
    candialog.h \
    canmessageedit.h \
    canrxworker.h \
    lcarddialog.h \
    lcard/LusbapiTypes.h \
    lcardworker.h \
    video/fvcamerastateinfo.h \
    video/fvtypes.h \
    video/MovieFileParams.h \
    video/view.h

FORMS    += mainwindow.ui \
    candialog.ui \
    lcarddialog.ui

win32: LIBS += -L$$PWD/vscan/ -lvs_can_api

INCLUDEPATH += $$PWD/vscan
DEPENDPATH += $$PWD/vscan

win32: LIBS += -L$$PWD/lcard/ -lLusbapi

INCLUDEPATH += $$PWD/lcard
DEPENDPATH += $$PWD/lcard

OTHER_FILES +=

RESOURCES += \
    resource.qrc

RC_FILE += \
    koeikd.rc
