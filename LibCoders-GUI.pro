#
# libcoders.pro
#
# Part of LibCoders-GUI
# by snovvcrash
# 07.2017
#

#--------------------------------------
#
# Project created by QtCreator 2017-07
#
#--------------------------------------

QT             += core gui
QMAKE_CXXFLAGS += -std=c++0x -O2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
                                     concurrent

TARGET   = LibCoders-GUI.exec
TEMPLATE = app


SOURCES += main.cxx        \
        mainwindow.cxx     \
        helpdialog.cxx     \
        aboutdialog.cxx    \
        coders/acoder.cxx  \
        coders/ahcoder.cxx \
        coders/bhcoder.cxx \
        coders/pcoder.cxx

HEADERS += mainwindow.hxx  \
        helpdialog.hxx     \
        aboutdialog.hxx    \
        coders/acoder.hxx  \
        coders/ahcoder.hxx \
        coders/bhcoder.hxx \
        coders/pcoder.hxx

FORMS   += mainwindow.ui \
        helpdialog.ui    \
        aboutdialog.ui
