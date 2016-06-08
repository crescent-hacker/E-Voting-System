#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T16:35:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CSCI969Server
TEMPLATE = app
CONFIG += c++11
CONFIG += console

INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /usr/local/opt/openssl/include/
INCLUDEPATH += /usr/local/Cellar/log4cplus/1.2.0/include/

LIBS += -L"/usr/local/Cellar/log4cplus/1.2.0/lib/" -llog4cplus
LIBS += -L"/usr/local/opt/openssl/lib/" -lssl -lcrypto
LIBS += -L"/usr/local/lib/" -lboost_system
LIBS += -L"/usr/local/lib/" -lboost_graph

SOURCES += main.cpp\
        loginwindow.cpp \
    pvdialog.cpp \
    adddialog.cpp \
    fvdialog.cpp \
    mainwin.cpp \
    clienthandler.cpp \
    readconf.cpp \
    user.cpp \
    usermainwindow.cpp \
    adduserdialog.cpp \
    vote.cpp \
    modifypwddialog.cpp \
    uservotedialog.cpp

HEADERS  += loginwindow.h \
    pvdialog.h \
    adddialog.h \
    fvdialog.h \
    mainwin.h \
    client_http.hpp \
    client_https.hpp \
    clienthandler.h \
    readconf.h \
    user.h \
    usermainwindow.h \
    adduserdialog.h \
    vote.h \
    modifypwddialog.h \
    uservotedialog.h

FORMS    += loginwindow.ui \
    pvdialog.ui \
    adddialog.ui \
    fvdialog.ui \
    mainwin.ui \
    usermainwindow.ui \
    adduserdialog.ui \
    modifypwddialog.ui \
    uservotedialog.ui
