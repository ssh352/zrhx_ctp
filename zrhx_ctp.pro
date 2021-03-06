#-------------------------------------------------
#
# Project created by QtCreator 2017-06-07T16:24:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zrhx_ctp
TEMPLATE = app
RC_FILE=icon.rc


SOURCES += main.cpp\
    mainwindow.cpp \
    ctp_manager.cpp \
    ctp_quote.cpp \
    ctp_trade.cpp \
    ../gpp_qt/cfg/cfg.cpp \
    ../gpp_qt/cmd_line/cmd_line.cpp \
    ../gpp_qt/wfunction/wfunction.cpp \
    ctp_order_manager.cpp \
    ctp_order.cpp \
    position.cpp

HEADERS  += mainwindow.h \
    ctp_manager.h \
    ctp_quote.h \
    ctp_trade.h \
    ../gpp_qt/cfg/cfg.h \
    ../gpp_qt/wtimer/wtimer.h \
    ../gpp_qt/cmd_line/cmd_line.h \
    ../gpp_qt/wfunction/wfunction.h \
    ../libs/ctp/ThostFtdcMdApi.h \
    ../libs/ctp/ThostFtdcTraderApi.h \
    ../libs/ctp/ThostFtdcUserApiDataType.h \
    ../libs/ctp/ThostFtdcUserApiStruct.h \
    ctp_order_manager.h \
    ctp_order.h \
    position.h

FORMS    += mainwindow.ui



win32: LIBS += -L$$PWD/../libs/ctp/ -lthostmduserapi
win32: LIBS += -L$$PWD/../libs/ctp/ -lthosttraderapi

INCLUDEPATH += $$PWD/../libs/ctp
DEPENDPATH += $$PWD/../libs/ctp
