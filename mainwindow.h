#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QApplication>
#include<QLabel>
#include<QMessageBox>

#include<string>
#include<map>
#include<memory>
#include"../libs/ctp/ThostFtdcMdApi.h"
class ctp_manager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();
    void set_order_send(const std::string &);
    void set_seperate_label();
    void set_symbols_display(const std::string &);

    void show_string_quote(const std::string &);
    void show_string_trade(const std::string &);
    void ctr_price_display(const std::string & , double);

    void set_qa(QApplication * p){qa=p;}

signals:
    void on_pushButton_6_clicked();
    void on_pushButton_16_clicked();
    void check_position();
    void run_order_file(const std::string &);


    void check_add_order(const std::string &,const std::string &,const std::string &,const std::string &,const std::string &);

public slots:
    void show_quote_label(std::shared_ptr<CThostFtdcDepthMarketDataField>);
    void on_pushButton_5_clicked();


private slots:
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();
//    void on_pushButton_3_clicked();
//    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    int get_prec(std::string);

    Ui::MainWindow *ui;
    std::map<std::string,QLabel *> quote_labels;

    QApplication * qa;
    std::string orderfile;
};

#endif // MAINWINDOW_H
