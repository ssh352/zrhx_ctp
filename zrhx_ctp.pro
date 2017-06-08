#-------------------------------------------------
#
# Project created by QtCreator 2017-06-07T16:24:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zrhx_ctp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ctp_manager.cpp \
    tactic.cpp \
    ctp_quote.cpp \
    ../gpp_qt/cfg/cfg.cpp \
    ../gpp_qt/cmd_line/cmd_line.cpp \
    ../gpp_qt/wfunction/wfunction.cpp

HEADERS  += mainwindow.h \
    ctp_manager.h \
    tactic.h \
    ctp_quote.h \
    ../gpp_qt/cfg/cfg.h \
    ../gpp_qt/wtimer/wtimer.h \
    ../libs/ctp/ThostFtdcMdApi.h \
    ../libs/ctp/ThostFtdcTraderApi.h \
    ../libs/ctp/ThostFtdcUserApiDataType.h \
    ../libs/ctp/ThostFtdcUserApiStruct.h \
    ../gpp_qt/cmd_line/cmd_line.h \
    ../gpp_qt/wfunction/wfunction.h

FORMS    += mainwindow.ui



win32: LIBS += -L$$PWD/../libs/ctp/ -lthostmduserapi

INCLUDEPATH += $$PWD/../libs/ctp
DEPENDPATH += $$PWD/../libs/ctp
