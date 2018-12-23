#-------------------------------------------------
#
# Project created by QtCreator 2018-10-27T18:47:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:VERSION = 1.0.0.358 # major.minor.patch.build
else:VERSION = 1.0.0    # major.minor.patch

QMAKE_TARGET_PRODUCT = DeviceReport
QMAKE_TARGET_COMPANY = Mykola Demchuk
QMAKE_TARGET_COPYRIGHT = Mykola Demchuk
RC_ICONS = res/icon.ico
RC_LANG = 0x0C09
RC_CODEPAGE = 1252

TARGET = DeviceReport
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdlg.cpp

HEADERS  += mainwindow.h \
    aboutdlg.h

FORMS    += mainwindow.ui \
    aboutdlg.ui

RESOURCES += \
    res.qrc

DISTFILES +=
