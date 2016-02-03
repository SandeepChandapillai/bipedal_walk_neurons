#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T16:24:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LIPM_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lipm_model.cpp \
    network.cpp \
    neuron.cpp \
    cnb.cpp

HEADERS  += mainwindow.h \
    lipm_model.h \
    network.h \
    neuron.h \
    cnb.h

FORMS    += mainwindow.ui


# With C++11 support
greaterThan(QT_MAJOR_VERSION, 4){
CONFIG += c++11
} else {
QMAKE_CXXFLAGS += -std=c++0x
}
