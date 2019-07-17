#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T00:07:55
#
#-------------------------------------------------
QT += multimedia multimediawidgets serialport
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = helloworld
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MineMap.cpp \
    mainmeun.cpp \
    music.cpp \
    sdmmc.cpp \
    systimecontrol.cpp \
    uart.cpp \
    usb.cpp \
    videowidget.cpp \
    ledcontrol.cpp

HEADERS  += mainwindow.h \
    MineMap.h \
    mainmeun.h \
    music.h \
    sdmmc.h \
    systimecontrol.h \
    uart.h \
    usb.h \
    videowidget.h \
    ledcontrol.h

FORMS    += mainwindow.ui \
    mainmeun.ui \
    music.ui \
    sdmmc.ui \
    systimecontrol.ui \
    uart.ui \
    usb.ui \
    videowidget.ui \
    ledcontrol.ui

RESOURCES += \
    hellores.qrc \
    static.qrc \
    res.qrc

DISTFILES += \
    README.md
