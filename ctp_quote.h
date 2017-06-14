#ifndef CTP_QUOTE
#define CTP_QUOTE


#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<list>
#include<memory>
#include<windows.h>

#include "mainwindow.h"
#include<QApplication>
#include<QThread>

#include"../gpp_qt/cfg/cfg.h"
#include"../gpp_qt/log_info/log_info.h"
#include"../gpp_qt/wtimer/wtimer.h"
#include"../gpp_qt/bar/bars_manage.h"
#include"../gpp_qt/wfunction/wfunction.h"

#include"../libs/ctp/ThostFtdcMdApi.h"

#define MAX_CONTRACT_NUMBER 1024
#define MAX_CONTRACT_NAME 1024

class ctp_quote_qthread;

class ctp_quote : public QThread, CThostFtdcMdSpi
{
    Q_OBJECT
public:
    void init();
    char * mk_quote_con_dir();
    void login();
	void ReqUserLogin();

    void OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast);
	void OnFrontDisconnected(int nReason);
	void OnFrontConnected();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void SubscribeMarketData();
    void SubscribeMarketData(char **,int);

    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

signals:
//    void broadcast_quote(const std::string &symbol, const std::string &ba, long level, double price, long size);
    void broadcast_quote(std::shared_ptr<CThostFtdcDepthMarketDataField>);

private:
    void run(){this->init();}

	CThostFtdcReqUserLoginField * req;
    CThostFtdcMdApi * pUserApi;
    int nRequestID;
	int nppInstrumentID;
    char ** ppInstrumentID;

    int ctp_time_length;
};

#endif
