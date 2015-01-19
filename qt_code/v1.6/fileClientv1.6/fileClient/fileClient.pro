#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T23:13:09
#
#-------------------------------------------------

QT       += core gui network

TARGET = fileClient
TEMPLATE = app


SOURCES += main.cpp\
        tcpclientfilesend.cpp \
    threadsd.cpp

HEADERS  += tcpclientfilesend.h \
    threadsd.h

FORMS    += tcpclientfilesend.ui

LIBS += -lwsock32 -lpthread
RC_FILE = logo.rc
