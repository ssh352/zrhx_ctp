#include"ctp_manager.h"
#include"../gpp_qt/cfg/cfg.h"
#include "mainwindow.h"

extern cfg cfg_info;

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
    quote->start();
}
void ctp_manager::start_ctp_trade()
{
}
void ctp_manager::from_ctp_quote(const string & quotestr)
{
    cerr<<"from_ctp_quote\t"<<quotestr<<endl;
}

//From tactic


