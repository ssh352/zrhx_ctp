#include"ctp_manager.h"
#include"windows.h"

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

}
void ctp_manager::start_ctp_trade()
{
    om=new ctp_order_manager;
    trade=new ctp_trade;

    om->set_mw(mw);
    om->set_trade(trade);
    om->init();

    QObject::connect(trade, &ctp_trade::OnLogin, om, &ctp_order_manager::OnLogin);
    QObject::connect(mw, &MainWindow::check_add_order, om, &ctp_order_manager::check_add_order);
    QObject::connect(mw, &MainWindow::run_order_file, om, &ctp_order_manager::run_order_file);

    trade->start();
//    这部分是进行了下单测试  成功
//    Sleep(1000);
//    om->new_order("IF1710","BUY","OPEN",4000,1);
//    Sleep(1000);
//    om->new_order("IF1711","BUY","OPEN",4000,1);
}
void ctp_manager::from_ctp_quote(std::shared_ptr<CThostFtdcDepthMarketDataField> squote)
{
    //此处已经测试成功
    //cerr<<"from_ctp_quote\t"<<(*squote).InstrumentID<<"\tLastPrice\t"<<(*squote).LastPrice<<"\t"<<(*squote).AskVolume1<<endl;
}


//From tactic



