#-------------------------------------------------
#
# Project created by QtCreator 2017-07-21T15:48:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudDisk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    logindial.cpp \
    loginupper.cpp \
    userinfo.cpp \
    mainheader.cpp \
    mainwindow.cpp \
    common.cpp \
    threadpool.c \
    uploadparam.cpp \
    httpresponse.cpp

HEADERS  += \
    logindial.h \
    loginupper.h \
    userinfo.h \
    mainheader.h \
    mainwindow.h \
    common.h \
    threadpool.h \
    uploadparam.h \
    httpresponse.h

FORMS    += \
    logindial.ui \
    loginupper.ui \
    mainheader.ui \
    mainwindow.ui

RESOURCES += \
    resource.qrc
