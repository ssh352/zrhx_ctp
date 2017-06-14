#include"ctp_manager.h"

extern cfg cfg_info;

using namespace std;

void ctp_manager::init()
{
    start_ctp_quote();
    start_ctp_trade();

}
//From ctp
bool ctp_manager::check_trade_init_para()
{
    return true;
}
bool ctp_manager::check_quote_init_para()
{
    return true;
}
void ctp_manager::start_ctp_quote()
{
    quote=new ctp_quote;
    QObject::connect(quote, &ctp_quote::broadcast_quote, this, &ctp_manager::from_ctp_quote);
    QObject::connect(quote, &ctp_quote::broadcast_quote, mw, &MainWindow::show_quote_label);
    quote->start();

    trade=new ctp_trade;
    trade->start();
}
void ctp_manager::start_ctp_trade()
{
}
void ctp_manager::from_ctp_quote(std::shared_ptr<CThostFtdcDepthMarketDataField> squote)
{
    cerr<<"from_ctp_quote\t"<<(*squote).InstrumentID<<"\tLastPrice\t"<<(*squote).LastPrice<<"\t"<<(*squote).AskVolume1<<endl;
}


//From tactic



