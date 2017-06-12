#include "ctp_trade.h"

#include<Windows.h>
#include<string>
#include<memory>

#include<QApplication>

#include"../gpp_qt/wfunction/wfunction.h"
#include"../gpp_qt/cfg/cfg.h"

extern cfg cfg_info;

using namespace std;

void ctp_trade::init()
{
    iRequestID=0;
    cout<<"init trade"<<endl;
    maxdelaytime=atoi(cfg_info.get_para("MAX_QUERY_DELAY").c_str());
    pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    pUserApi->RegisterSpi((CThostFtdcTraderSpi*)this);
    pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
    pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    pUserApi->RegisterFront(const_cast<char*>(cfg_info.get_para("TRADE_FRONT_ADDR").c_str()));// connect
    pUserApi->Init();
    pUserApi->Join();
}
void ctp_trade::ReqUserLogin()
{
    CThostFtdcReqUserLoginField ulreq;
    memset(&ulreq, 0, sizeof(ulreq));
    strncpy(ulreq.BrokerID,const_cast<char*>(cfg_info.get_para("BROKER_ID").c_str()),sizeof(ulreq.BrokerID));
    strncpy(ulreq.UserID,const_cast<char*>(cfg_info.get_para("INVESTOR_ID").c_str()),sizeof(ulreq.UserID));
    strncpy(ulreq.Password,const_cast<char*>(cfg_info.get_para("PASSWORD").c_str()),sizeof(ulreq.Password));
    int iResult = pUserApi->ReqUserLogin(&ulreq, ++iRequestID);
    cerr << "--->>> Sending login request: " << iResult << ((iResult == 0) ? ",Successed" : ",Fail") << endl;
}
void ctp_trade::ReqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField screq;
    memset(&screq, 0, sizeof(screq));
    strncpy(screq.BrokerID, const_cast<char*>(cfg_info.get_para("BROKER_ID").c_str()), sizeof(screq.BrokerID));
    strncpy(screq.InvestorID, const_cast<char*>(cfg_info.get_para("INVESTOR_ID").c_str()), sizeof(screq.InvestorID));
    int iResult = pUserApi->ReqSettlementInfoConfirm(&screq, ++iRequestID);
    cerr << endl << "--->>> Confirm settlement: " << iResult << ((iResult == 0) ? ",Successed" : ",Fail") << endl;
}
void ctp_trade::ReqQryOrder(const string & instrument_id)
{
    ReqQryOrder(instrument_id,false);
}
void ctp_trade::ReqQryOrder(const string & instrument_id,bool fast)
{
    CThostFtdcQryOrderField ofreq;
    memset(&ofreq, 0, sizeof(ofreq));
    strncpy(ofreq.BrokerID,const_cast<char*>(cfg_info.get_para("BROKER_ID").c_str()),sizeof(ofreq.BrokerID));
    strncpy(ofreq.InvestorID,const_cast<char*>(cfg_info.get_para("INVESTOR_ID").c_str()),sizeof(ofreq.InvestorID));
    strncpy(ofreq.InstrumentID,const_cast<char*>(instrument_id.c_str()),sizeof(ofreq.InstrumentID));
    int delaytime=0;
    while (true)
    {
        if(delaytime>maxdelaytime)
        {
            cerr<<"--->>> �����ѯ����ʱ�䳬�������ʱ\t"<<"�����Լ:\t"<<instrument_id<<"\t�����ʱ(second):\t"<<maxdelaytime<<endl;
            break;
        }
        int iResult = pUserApi->ReqQryOrder(&ofreq, ++iRequestID);
        if (!IsFlowControl(iResult))
        {
            cerr << "--->>> �����ѯ����: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
            break;
        }
        else
        {
            if(fast)
            {
                cerr << "--->>> �����ѯ����: "  << iResult << ", �ܵ�����\t fast ģʽ,������ѯ" << endl;
                break;
            }
            else
            {
                cerr << "--->>> �����ѯ����: "  << iResult << ", �ܵ�����\t ��ͨģʽ���ȴ�һ��������ѯ" << endl;
                Sleep(1000);
                delaytime++;
            }
        }
    }
}
void ctp_trade::ReqQryInstrument(const string & instrument_id)
{
    ReqQryInstrument(instrument_id,false);//defaule mode is "wait untill get result"
}
void ctp_trade::ReqQryInstrument(const string & instrument_id,bool fast)
{
    CThostFtdcQryInstrumentField qireq;
    memset(&qireq, 0, sizeof(qireq));
    strcpy(qireq.InstrumentID,const_cast<char*>(instrument_id.c_str()));
    int delaytime=0;
    while (true)
    {
        //���κ�Qry���󣬳�����������ѭ���Ŀ���
        //Ŀǰ�����ʽ����cfg����ѯʱ�������� ���ڴ����ʱ��ʱ������沢����
        if(delaytime>maxdelaytime)
        {
            cerr<<"--->>> �����ѯ��Լʱ�䳬�������ʱ\t"<<"�����Լ:\t"<<instrument_id<<"\t�����ʱ:\t"<<maxdelaytime<<endl;
            break;
        }
        int iResult = pUserApi->ReqQryInstrument(&qireq, ++iRequestID);
        if (!IsFlowControl(iResult))
        {
            cerr << "--->>> �����ѯ��Լ: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
            break;
        }
        else
        {
            if(fast)
            {
                cerr << "--->>> �����ѯ��Լ: "  << iResult << ", �ܵ�����\t fast ģʽ,������ѯ" << endl;
                break;
            }
            else
            {
                cerr << "--->>> �����ѯ��Լ: "  << iResult << ", �ܵ�����\t ��ͨģʽ���ȴ�һ��������ѯ" << endl;
                Sleep(1000);
                delaytime++;
            }
        }
    }
}
void ctp_trade::ReqQryTradingAccount()
{
    ReqQryTradingAccount(false);
}
void ctp_trade::ReqQryTradingAccount(bool fast)
{
    CThostFtdcQryTradingAccountField tareq;
    memset(&tareq, 0, sizeof(tareq));
    strncpy(tareq.BrokerID,const_cast<char*>(cfg_info.get_para("BROKER_ID").c_str()),sizeof(tareq.BrokerID));
    strncpy(tareq.InvestorID,const_cast<char*>(cfg_info.get_para("INVESTOR_ID").c_str()),sizeof(tareq.InvestorID));
    int delaytime=0;
    while (true)
    {
        int iResult = pUserApi->ReqQryTradingAccount(&tareq, ++iRequestID);
        if(delaytime>maxdelaytime)
        {
            cerr<<"--->>> �����ѯ�ʽ��˻�ʱ�䳬�������ʱ\t"<<"������:\t"<<tareq.BrokerID<<"\t�����ʽ��˻�:\t"<<tareq.InvestorID<<"\t�����ʱ:\t"<<maxdelaytime<<endl;
            break;
        }
        if (!IsFlowControl(iResult))
        {
            cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
            break;
        }
        else
        {
            if(fast)
            {
                cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ", �ܵ�����\t fast ģʽ,������ѯ" << endl;
                break;
            }
            else
            {
                cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ", �ܵ�����\t ��ͨģʽ���ȴ�һ��������ѯ" << endl;
                Sleep(1000);
                delaytime++;
            }
        }
    }
}
void ctp_trade::ReqQryInvestorPosition(const string & instrument_id)
{
    ctp_trade::ReqQryInvestorPosition(instrument_id,false);
}
void ctp_trade::ReqQryInvestorPosition(const string & instrument_id,bool fast)
{
    CThostFtdcQryInvestorPositionField ipreq;
    memset(&ipreq, 0, sizeof(ipreq));
    ////// query all position
//    strncpy(ipreq.InstrumentID,const_cast<char*>(instrument_id.c_str()),sizeof(ipreq.InstrumentID));
    strncpy(ipreq.InstrumentID,"",0);
    strncpy(ipreq.BrokerID,const_cast<char*>(cfg_info.get_para("BROKER_ID").c_str()),sizeof(ipreq.BrokerID));
    strncpy(ipreq.InvestorID,const_cast<char*>(cfg_info.get_para("INVESTOR_ID").c_str()),sizeof(ipreq.InvestorID));
    int delaytime=0;
    while (true)
    {
        int iResult = pUserApi->ReqQryInvestorPosition(&ipreq, ++iRequestID);
        if(delaytime>maxdelaytime)
        {
            cerr<<"--->>> �����ѯ�ֲ�ʱ�䳬�������ʱ\t"<<"��Լ:\t"<<ipreq.InvestorID<<"\t�����ʱ:\t"<<maxdelaytime<<endl;
            break;
        }
        if (!IsFlowControl(iResult))
        {
            cerr << "--->>>  �����ѯ�ֲ� Request Position: "  << iResult << ((iResult == 0) ? ", success " : ", fali") << endl;
            break;
        }
        else
        {
            if(fast)
            {
                cerr << "--->>> �����Լ�ֲ�: "  << iResult << ", �ܵ�����\t fast ģʽ,������ѯ" << endl;
                break;
            }
            else
            {
                cerr << "--->>> �����ѯ��Լ�ֲ�: "  << iResult << ", �ܵ�����\t ��ͨģʽ���ȴ�һ��������ѯ" << endl;
                Sleep(1000);
                delaytime++;
            }
        }
    }
}
void ctp_trade::ReqOrderInsert(CThostFtdcInputOrderField * porder)
{
    cerr <<endl << "--->>>ReqOrderInsert" << endl;
    cerr<<"iRequeseID in  p "<<porder->RequestID<<endl;
    int iResult = pUserApi->ReqOrderInsert(porder, porder->RequestID);
    if(iResult==0)
    {
        cerr << endl << "--->>> order insert: " << iResult << " Success" << endl;
    }
    else
    {
        cerr << endl << "--->>> order insert: " << iResult << " Fail" << endl;
    }
}
void ctp_trade::ReqOrderAction(CThostFtdcInputOrderActionField *pOrder)
{
    cerr << endl << "--->>> ReqOrderAction" <<endl;
    int iResult = pUserApi->ReqOrderAction(pOrder, ++iRequestID);
    cerr << "ReqOrderAction : " << iResult << ((iResult == 0) ? ", success" : ", fail") << endl;
    cerr << "ReqOrderAction :  ID " << iRequestID << " InstrumentID: " << pOrder->InstrumentID << endl;
}
void ctp_trade::ReqForQuoteInsert(CThostFtdcInputForQuoteField *porder, int nRequestID)
{
    cerr << endl << "--->>> ReqForQuoteInsert" <<endl;
    cerr<<"iRequeseID:\t "<<nRequestID<<endl;
    int iResult = pUserApi->ReqForQuoteInsert(porder, nRequestID);
    if(iResult==0)
    {
        cerr << endl << "--->>> req quote ask: " << iResult << " Success" << endl;
    }
    else
    {
        cerr << endl << "--->>> req quote ask: " << iResult << " Fail" << endl;
    }
}
void ctp_trade::ReqQuoteInsert(CThostFtdcInputQuoteField *porder)
{
    cerr << endl << "--->>> ReqQuoteInsert" <<endl;
    cerr<<"iRequeseID:\t "<<porder->RequestID<<endl;
    int iResult = pUserApi->ReqQuoteInsert(porder, porder->RequestID);
    if(iResult==0)
    {
        cerr << endl << "--->>> send quote: " << iResult << " Success" << endl;
    }
    else
    {
        cerr << endl << "--->>>  send quote: " << iResult << " Fail" << endl;
    }
}
void ctp_trade::ReqQuoteAction(CThostFtdcInputQuoteActionField *porder)
{
    cerr << endl << "--->>> ReqQuoteAction" <<endl;
    cerr<<"iRequeseID:\t "<<porder->RequestID<<endl;
    int iResult = pUserApi->ReqQuoteAction(porder, porder->RequestID);
    if(iResult==0)
    {
        cerr << endl << "--->>> change quote: " << iResult << " Success" << endl;
    }
    else
    {
        cerr << endl << "--->>>  change quote: " << iResult << " Fail" << endl;
    }
}
char *ctp_trade::mk_trade_con_dir()
{
        string exedir=cfg_info.get_para("TRADE_CON_PATH");
        if(exedir.size()>0)
        {
            wfunction::wmkdir(exedir);
            return const_cast<char*>((exedir+"/").c_str());
        }
        else
        {
            exedir=QCoreApplication::applicationFilePath().toStdString();
            cout<<exedir<<endl;
            exedir=exedir.erase(exedir.find_last_of("/"),exedir.size());
//            exedir=exedir.erase(exedir.find_last_of("\\"),exedir.size());
            exedir=exedir+"/trade_con";
            wfunction::wmkdir(exedir);
        }
        return const_cast<char*>((exedir+"/").c_str());
}
//////////////////////////////////////////////////////////////////////////////////////////////
//
//Functions called from exchange.
//
//////////////////////////////////////////////////////////////////////////////////////////////

void ctp_trade::OnFrontConnected()
{
    cerr << "--->>> " << "OnFrontConnected" << endl;
    ReqUserLogin();
}
void ctp_trade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    cerr <<endl << "--->>> " << "OnRspUserLogin" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //save para
        FRONT_ID = pRspUserLogin->FrontID;
        SESSION_ID = pRspUserLogin->SessionID;
        strcpy(this->MaxOrderRef,pRspUserLogin->MaxOrderRef);
        cerr<<"FRONT_ID init "<<FRONT_ID<<endl;
        cerr<<"SESSION_ID init "<<SESSION_ID<<endl;
        cerr<<"--->>>  MaxOrderRef "<<pRspUserLogin->MaxOrderRef<<endl;
        cerr<<"--->>> get exchange trading day = " << pUserApi->GetTradingDay() << endl;
        CThostFtdcRspUserLoginField tp;
        memcpy(&tp,pRspUserLogin,sizeof(CThostFtdcRspUserLoginField));
        emit OnLogin(&tp);
        ReqSettlementInfoConfirm();
    }
}
void ctp_trade::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << endl << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
    cerr << "BrokerID:\t"<<pSettlementInfoConfirm->BrokerID<<"\t"<<"nRequestID:\t"<<nRequestID<<endl;
    if (bIsLast)
    {
        IsErrorRspInfo(pRspInfo);
    }
}
void ctp_trade::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//���޴�����
    cerr << endl << "--->>> " << "OnRspQryInstrument" << endl;
    cerr << pInstrument->InstrumentID << " "<< pInstrument->InstrumentName <<endl;
    cerr << "nRequtestID" << nRequestID << endl;
    if ( bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //�ڴ����ú�Լ����
		//��Ҫ����ÿ�η��ص�ָ���Ƿ�һ�£�Ҳ�����Ƿ���Ҫcopy InstrumentField������
        cerr << "qryInstrument done" << endl;
    }
}
void ctp_trade::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//���޴�����
	cout << pOrder->OrderStatus << pOrder->OrderRef << pOrder->OrderSysID << endl;
	if (bIsLast)
	{
		IsErrorRspInfo(pRspInfo);
	}
}
void ctp_trade::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//���޴�����
	cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast)
	{
		IsErrorRspInfo(pRspInfo);
	}
}
void ctp_trade::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    this->ReqQryInstrument("");
    cerr << "--->>> " << "OnRspQryInvestorPosition" << endl;
    cerr <<  "bIsLast\t"<<bIsLast << endl;
    cerr <<  "nRequestID\t"<<nRequestID << endl;
    if(pInvestorPosition==nullptr){cerr<<"POS NUL"<<endl;}
    if(pRspInfo==NULL){cerr<<"INFO NUL"<<endl;}
    if(pInvestorPosition==NULL && pRspInfo==NULL){return;}
    cerr <<"--->>> ErrorID\t" <<pRspInfo->ErrorID<<endl;
    cerr <<"--->>> ErrorMsg\t" <<pRspInfo->ErrorMsg<<endl;
    {
        cerr <<"InstrumentID\n"<<pInvestorPosition->InstrumentID<<endl;
        cerr <<"BrokerID\n"<<pInvestorPosition->BrokerID<<endl;
        cerr <<"InvestorID\n"<<pInvestorPosition->InvestorID<<endl;
        cerr <<"Position\n"<<pInvestorPosition->Position<<endl;
        cerr <<"TodayPosition\n"<<pInvestorPosition->TodayPosition<<endl;
        cerr <<"PosiDirection\n"<<pInvestorPosition->PosiDirection<<endl;

    }
	if (bIsLast)
	{
		IsErrorRspInfo(pRspInfo);
	}
    //////////check all instrument

}
void ctp_trade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    //���޴�����
    //�������ִ���ʱ�Ż���ô˺���
    cerr << "--->>> " << "OnRspOrderInsert" << endl;
    cerr << "--->>> " << "Thost parameter check fail" << endl;
	if (bIsLast)
	{
		IsErrorRspInfo(pRspInfo);
	}
}
void ctp_trade::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//���޴�����
	cerr << "--->>> " << "OnRspOrderAction" << endl;
	if (bIsLast)
	{
		IsErrorRspInfo(pRspInfo);
	}
}
void ctp_trade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}
void ctp_trade::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
	cerr << "���ӶϿ�ԭ��" << endl;
	cerr << "0x1001 ���� �� ʧ��" << endl;
	cerr << "0x1002 ���� д ʧ��" << endl;
	cerr << "0x2001 ���� ���� ��ʱ" << endl;
	cerr << "0x2002 ���� ���� ʧ��" << endl;
	cerr << "0x2003 �յ� ���� ����" << endl;
}
void ctp_trade::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
//Bool Functions
//
//////////////////////////////////////////////////////////////////////////////////////////////
bool ctp_trade::IsFlowControl(int iResult)
{
    return ((iResult == -2) || (iResult == -3));
}
bool ctp_trade::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	}
	return bResult;
}
bool ctp_trade::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	//	return (
	//		(pOrder->FrontID == FRONT_ID) &&
	//		(pOrder->SessionID == SESSION_ID) &&
	//		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));

	//Ŀǰ��δʹ��
	return true;
}
bool ctp_trade::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	//	return (
	//		(pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
	//		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
	//		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
	
	//Ŀǰ��δʹ��
	return true;
}

//���Կ�֪ ֱ�Ӵ���ָ��p �ᵼ��OrderStatus�ֶβ�����
void ctp_trade::OnRtnOrder(CThostFtdcOrderField *p)
{
    CThostFtdcOrderField * tmpp=new CThostFtdcOrderField;
    memcpy(tmpp,p,sizeof(CThostFtdcOrderField));
    emit this->send_rtn_order(tmpp);

//    shared_ptr<CThostFtdcDepthMarketDataField> squote(new CThostFtdcDepthMarketDataField);
//    memcpy(&(*squote),p,sizeof(*p));
//    emit broadcast_quote(squote);
}

void ctp_trade::OnRtnTrade(CThostFtdcTradeField *p)
{
    CThostFtdcTradeField * tmpp=new CThostFtdcTradeField;
    memcpy(tmpp,p,sizeof(CThostFtdcTradeField));
    emit this->send_rtn_trade(tmpp);

//    shared_ptr<CThostFtdcDepthMarketDataField> squote(new CThostFtdcDepthMarketDataField);
//    memcpy(&(*squote),p,sizeof(*p));
//    emit broadcast_quote(squote);
}

