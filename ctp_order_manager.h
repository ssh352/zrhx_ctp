#ifndef CTP_ORDER_MANAGER_H
#define CTP_ORDER_MANAGER_H

#include"ctp_order.h"
#include"ctp_trade.h"

#include<list>
#include<map>
#include<string>

#include<QMutex>
#include<QObject>

#include"../libs/ctp/ThostFtdcTraderApi.h"
#include"mainwindow.h"

class ctp_order_manager :public QObject
{
    Q_OBJECT
public:
    void init();
    void set_trade(ctp_trade * p){trade=p;}
    void set_mw(MainWindow * p){mw=p;}

    std::string new_order(const std::string symbol,const std::string buysell, const std::string & openclose ,double price,long size);
    void cancel_order(const std::string & ordername);
    void cancel_all_order();
    void test_change_order();
    void change_order(const std::string & ordername,double price,long size);
    //还可加入查询order状态的函数

    TThostFtdcFrontIDType get_front_id(){return FRONT_ID;}
    TThostFtdcSessionIDType get_session_id(){return SESSION_ID;}
    void  add_OrderRef();

public slots:
    void check_add_order(const std::string & ID,const std::string & side ,const std::string & openclose ,const std::string & price ,const std::string & size);
    void show_warning(const std::string &);
    void rec_quote(const std::string & symbol, const std::string & ba, long level, double price, long size);
    void rec_book(const CThostFtdcDepthMarketDataField *pDepthMarketData);

    void OnRtnOrder(CThostFtdcOrderField *pOrder);
    void OnRtnTrade(CThostFtdcTradeField *pTrade);
    void OnLogin(CThostFtdcRspUserLoginField *pRspUserLogin);

signals:
    void ack(const std::string & ordername,const std::string & type,const std::string & info);
    void done(const std::string & ordername,const std::string & type,const std::string & info);
    void rej(const std::string & ordername,const std::string & type,const std::string & info);
    void fill(const std::string & ordername,const std::string & symbol,const std::string & buy_sell,double price, long size);
    void send_quote(const std::string & symbol, const std::string & ba, long level, double price, long size);
    void send_book(const CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
    std::map <std::string, std::list<long>> _ordername_iRequestID;
    std::map <std::string, ctp_order *> _ordername_order;
    std::map <long, std::string> _iRequestID_ordername;
    std::map <std::string,std::string > _fill_ordername;

    long _ordercount;

    TThostFtdcFrontIDType    FRONT_ID;
    TThostFtdcSessionIDType SESSION_ID;
    TThostFtdcOrderRefType  MaxOrderRef;
    TThostFtdcOrderRefType  NowOrderRef;
    int nowref;
    int _order_ref_sz;

    ctp_trade * trade;
    MainWindow * mw;
    CThostFtdcInputOrderField * initorder(const std::string & InstrumentID, const std::string & side, const std::string & openclose, double price, long size);
    CThostFtdcInputOrderActionField * initorderchange(const std::string & ordername);

    QMutex mutex;

    std::map<std::string,int> ctr_pos; //ctr position  now
    std::map<std::string,int> ctr_pos_to; //ctr position
    std::map<std::string,int> ctr_pos_dif; //ctr position
};

#endif


//待完成                  增添4个框 分别为 bid ask price size 其默认值为当前合约成交价的 + - 1    1
//增添三个个按钮     显示询价  发送报价 撤销报价



//对于多策略   可以采用包装的形式 将tactic与管理tactic的信息包装到一个类里面
//如
//tactic * tc;
//signals:
//quote();
//fill()
//....

//private:
//    tactic name;
//    quote_symbols;

// 以此来筛选将那些信息发送到tactic 报单信息 quote信息等



