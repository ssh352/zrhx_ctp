/****************************************
 *@system CTP下单系统
 *@company futures
 *@file ctp_manager.h
 *@brief 包装ctp操作
 *@history
 *20141219 王德扬 创建此注释
 ****************************************/
#ifndef CTP_MANAGER_H
#define CTP_MANAGER_H

#include<string>
#include<memory>

#include<QObject>
#include"mainwindow.h"

#include"ctp_quote.h"
#include"ctp_trade.h"
#include"../gpp_qt/cfg/cfg.h"

class ctp_manager:public QObject
{
    Q_OBJECT
public:
    void init();
    bool check_quote_init_para();
    bool check_trade_init_para();
    void start_ctp_quote();
    void start_ctp_trade();

    void set_mw(MainWindow * _mw){mw=_mw;}


public slots:
    //from ctp
    void from_ctp_quote(std::shared_ptr<CThostFtdcDepthMarketDataField>);//收到行情
    void from_ctp_ack(const std::string &){}  //收到确认
    void from_ctp_fill(const std::string &){} //收到成交
    void from_ctp_rej(const std::string &){}  //收到拒绝

    //from tactic
    void from_tc_add(const std::string &){} //收到发单
    void from_tc_cxl(const std::string &){} //收到撤单

signals:
    //to ctp
    void to_ctp_pos();               //检查持仓


    //to tactic
    void to_tc_quote(const std::string &);//发送行情数据
    void to_tc_ack(const std::string &);  //发送确认
    void to_tc_fill(const std::string &); //发送成交
    void to_tc_rej(const std::string &);  //发送拒绝

    void to_tc_run();
    void to_tc_pause();
    void to_tc_resume();

    //to mw
    void to_mw_quote(const std::string &);//发送行情数据

private:

    ctp_quote * quote;

    MainWindow *mw;
};

#endif // CTP_MANAGE_H
