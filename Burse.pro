#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T20:14:41
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Burse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        transaction.cpp \
        transactionprocessor.cpp \
        qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
        transaction.h  \
        transactionprocessor.h \
        qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui
