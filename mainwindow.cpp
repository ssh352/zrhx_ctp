#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include<QLabel>
#include<Qfont>
#include<QPalette>
#include<QSize>
#include<QLineEdit>

#include<map>
#include<iostream>
#include<string>
#include<list>
#include<iomanip>
#include<regex>

#include"../gpp_qt/cfg/cfg.h"
#include"../gpp_qt/wtimer/wtimer.h"
#include"../gpp_qt/wfunction/wfunction.h"

#include"ctp_manager.h"

using namespace std;

extern wtimer tm;
extern cfg cfg_info;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
void MainWindow::init()
{
    set_symbols_display(cfg_info.get_para("INSTRUMENT_ID"));
    set_order_send(cfg_info.get_para("INSTRUMENT_ID"));
    set_seperate_label();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_seperate_label()
{
    //ui->label_16->setStyleSheet("background-color:rgb(200,200,200)");
//    ui->label_17->setStyleSheet("background-color:rgb(200,200,200)");
//    ui->label_18->setStyleSheet("background-color:rgb(200,200,200)");
    //ui->label_19->setStyleSheet("background-color:rgb(200,200,200)");
}
void MainWindow::set_order_send(const std::string & symbols)
{
    this->ui->fontComboBox->clear();
    this->ui->fontComboBox->setFont(QFont("微软雅黑",9,-1,false));

    list<string> symbol_list=wfunction::splitstring(symbols);
    QStringList strList;
    for(list<string>::iterator iter=symbol_list.begin();iter!=symbol_list.end();iter++)
    {
        this->ui->fontComboBox->addItem(iter->c_str());
    }

    this->ui->fontComboBox_2->clear();
    this->ui->fontComboBox_2->setFont(QFont("微软雅黑",9,-1,false));
    QRegExp rxp("^([1-9][0-9]{0,10}|0\\.)\\.?[0-9]{0,4}$");
    QValidator * validatorp = new QRegExpValidator(rxp,0);
    this->ui->fontComboBox_2->lineEdit()->setValidator(validatorp);
    this->ui->fontComboBox_2->setCurrentText("3200");

    this->ui->fontComboBox_3->clear();
    this->ui->fontComboBox_3->setFont(QFont("微软雅黑",9,-1,false));
    QRegExp rxs("^[1-9][0-9]{0,10}$");
    QValidator * validators = new QRegExpValidator(rxs,0);
    this->ui->fontComboBox_3->lineEdit()->setValidator(validators);
    this->ui->fontComboBox_3->setCurrentText("1");


    this->ui->comboBox ->clear();
    this->ui->comboBox->setFont(QFont("微软雅黑",9,-1,false));
    strList = (QStringList() << "买入" << "卖出");
    this->ui->comboBox->addItems(strList);
//    this->ui->comboBox->addItem("买入");
//    this->ui->comboBox->addItem("卖出");

    this->ui->comboBox_2 ->clear();
    this->ui->comboBox_2->setFont(QFont("微软雅黑",9,-1,false));
    strList = (QStringList() << "开新仓" << "平今" << "平昨");
    this->ui->comboBox_2->addItems(strList);
//    this->ui->comboBox_2->addItem("开新仓");
//    this->ui->comboBox_2->addItem("平今");
//    this->ui->comboBox_2->addItem("平昨");

}
void MainWindow::set_symbols_display(const std::string & symbols)
{
    list<string> symbol_list=wfunction::splitstring(symbols);
    for(list<string>::iterator iter=symbol_list.begin();iter!=symbol_list.end();iter++)
    {
        QLabel * ql=new QLabel();
        this->ui->horizontalLayout->addWidget(ql);
        ql->setText(QString::fromStdString(*iter));
        QPalette * pe=new QPalette;
        pe->setColor(QPalette::WindowText,Qt::red);
        ql->setPalette(*pe);
        ql->setFont(QFont("微软雅黑",14,-1,false));
        quote_labels[*iter]=ql;
    }
}
void MainWindow::show_quote_label(std::shared_ptr<CThostFtdcDepthMarketDataField> squote)
{
    ctr_price_display((*squote).InstrumentID,(*squote).LastPrice);
}
void MainWindow::ctr_price_display(const string & ctr, double price)
{
    auto iter=quote_labels.find(ctr);

    if(iter!=quote_labels.end())
    {
        quote_labels[ctr]->setText(QString::fromStdString(ctr)+QString("\n%1" ).arg(price,0,'f',this->get_prec(ctr)));
        qa->processEvents();
    }
    else
    {
        cerr<<"Error: function MainWindow::symbol_display"<<endl;
    }
}


void MainWindow::show_string_quote(const string &text)
{
    this->ui->textBrowser->append(QString::fromStdString(text));
    qa->processEvents();
}

int MainWindow::get_prec(string ctr)
{
    static map<std::string,int> prec;

    if(prec.find(ctr)!=prec.end())
    {
        return prec[ctr];
    }
    else if(regex_match(ctr,regex("^(TF|T)\\d+")))
    {
        prec[ctr]=3;
        return 3;
    }
    else if(regex_match(ctr,regex("^(au|fb|bb)\\d+")))
    {
        prec[ctr]=2;
        return 2;
    }
    else if(regex_match(ctr,regex("^(IF|IH|IC|ZC|j|jm|i)\\d+")))
    {
        prec[ctr]=1;
        return 1;
    }
    else
    {
        prec[ctr]=0;
        return 0;
    }
}
