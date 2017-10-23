#ifndef CTP_TRADE
#define CTP_TRADE
#include<string>
#include<map>
#include<string>
#include<memory>
#include<Windows.h>

#include<QApplication>
#include<QThread>

#include"../gpp_qt/wfunction/wfunction.h"
#include"../gpp_qt/cfg/cfg.h"

#include "../libs/ctp/ThostFtdcTraderApi.h"

class ctp_trade : public QThread, public CThostFtdcTraderSpi
{
    Q_OBJECT
public:
//	ctp_trade();
    void init();//set con file  dir
    char * mk_trade_con_dir();
    int add_iRequestID(){return ++iRequestID;}
    int get_iRequestID(){return iRequestID;}

    virtual void OnFrontConnected();
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);

    void ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID);
    void ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote);
    void ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction);

    void ReqQryOrder(const std::string &);
    void ReqQryOrder(const std::string &,bool);
    void ReqQryInstrument(const std::string & );
    void ReqQryInstrument(const std::string & ,bool);
    void ReqQryInvestorPosition();
    void ReqQryInvestorPosition(bool);

	void ReqQryTradingAccount(bool);
    void ReqQryTradingAccount();
    bool IsFlowControl(int );
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
    bool IsMyOrder(CThostFtdcOrderField *pOrder);
    bool IsTradingOrder(CThostFtdcOrderField *pOrder);
    void ReqUserLogin();
    void ReqSettlementInfoConfirm();
	
	void ReqOrderInsert(CThostFtdcInputOrderField *);
    void ReqOrderAction(CThostFtdcInputOrderActionField *pOrder);
	CThostFtdcInputOrderActionField * initorderchange(const std::string & ordername);

    void OnRtnOrder(CThostFtdcOrderField *pOrder);
    void OnRtnTrade(CThostFtdcTradeField *pTrade);
signals:
    void show_warning(const std::string &);
    void OnLogin(CThostFtdcRspUserLoginField *pRspUserLogin);

    void to_com_ack(CThostFtdcOrderField *pOrder);  //收到
    void to_com_fil(CThostFtdcTradeField *pTrade);  //收到成交
    void to_com_pos(CThostFtdcInvestorPositionField *pPos);  //shoudaochicang



private:
    void run(){this->init();}

    CThostFtdcTraderApi * pUserApi;
    TThostFtdcFrontIDType FRONT_ID;
    TThostFtdcSessionIDType SESSION_ID;
    TThostFtdcOrderRefType MaxOrderRef;
    int maxdelaytime;
    int iRequestID;
    //可以添加iRequestID对应的map 以便于查询order状态
    std::map<std::string, std::string> ordername_orderid; //user set id -> uniqid
    std::map<std::string, CThostFtdcOrderField*> orderid_op; //uniqid -> orderfield
    std::map<long, std::string> rid_orderid; //requestid -> uniqid


};
#endif // CTP_TRADE_H
