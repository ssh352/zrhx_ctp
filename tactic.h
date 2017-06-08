#ifndef TACTIC_H
#define TACTIC_H
#include<QObject>
#include<string>
#include"../gpp_qt/wtimer/wtimer.h"

#include"../libs/ctp/ThostFtdcMdApi.h"

const int PRICESTEP=5;
const int LIMITSTEP=5;
const int ORDERSZ=1;
class tactic: public QObject
{
    Q_OBJECT
public:
//    void set_ctp_order_manager(ctp_order_manager * p){om=p;}
    void init();
    void send_order();
    void set_timer(wtimer * p){timer=p;}

public slots:
    virtual void book(const CThostFtdcDepthMarketDataField *p);
    virtual void quote(const std::string & symbol, const std::string & ba, long level, double price, long quotesize);
    virtual void ack(const std::string & ordername,const std::string & type,const std::string & info);
    virtual void done(const std::string & ordername,const std::string & type,const std::string & info);
    virtual void rej(const std::string & ordername,const std::string & type,const std::string & info);
    virtual void fill(const std::string & ordername,const std::string & symbol,const std::string & buysell,double price, long size);
    virtual void pause(){_pause=true;}
    virtual void resume(){_pause=false;}
private:
    bool _pause;
    int ordersize;
    int ordersize_1;
    double lasttradeprice;
    double lasttradeprice_1;
    wtimer * timer;
};

#endif // TACTIC_H
