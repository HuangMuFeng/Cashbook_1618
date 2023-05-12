#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMainWindow>
#include<QString>
#include<QLabel>
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include<QPushButton>
#include<QWidget>
#include<QHBoxLayout>
#include<QInputDialog>
#include"itemwidget.h"
#include<QDateTime>
#include<QListWidgetItem>
#include<QListView>
#include<QFile>
#include<QIODevice>
#include<QByteArray>
#include<QBarCategoryAxis>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tagWidgetLayout.setParent(ui->dailyBoardContent);
    connect(&user_login,&userLoginWindow::loginSuccess,this,&MainWindow::user_login_sucess);
    connect(&user_login,&userLoginWindow::exitLogin,this,&MainWindow::exit_Login);
    connect(&itemMark,&itemMark::conveyData,this,&MainWindow::addNewItemLabel);
    ui->edit_pushButton->hide();
    ui->delet_pushButton_2->hide();
    ui->stackedWidget->setCurrentIndex(0);
    ui->logined->hide();
}

MainWindow::~MainWindow()
{
    QSqlQuery query;//建立一个数据库的query
    QString sql1;
    sql1="insert into user_login(id,useraccount)values(NULL,'') ";
    query.exec(sql1);
    query.exec("select  id,useraccount from user_login where id >= 0");
    while (query.next()) {
        QString id = query.value(0).toString();
        useraccount = query.value(1).toString();
        qDebug()<<"mainwindow:"<<id<<useraccount;
    }
    delete ui;
}

void MainWindow::on_addItemButton_clicked()
{
    if(useraccount == "未登录")
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "请先登录");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    itemMark.show();
}

void MainWindow::addNewItemLabel(int money ,QString type,QString content,QString time){
    newWidget = new itemWidget(money,type,content,time,ui->itemListWidget);
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<id<<useraccount;
    }

    if(content!=NULL){
        QDateTime dtm;
        QString timestr = dtm.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QSqlQuery query;//建立一个数据库的query
        QString sql1;
        sql1=QString("insert into item(id, money, type,content,addtime,useraccount) values(NULL,'%1', '%2', '%3','%4','%5')").arg(money).arg(type).arg(content).arg(timestr).arg(useraccount);
        query.exec(sql1);
        qDebug()<<money<<type<<content<<timestr<<useraccount;
        QListWidgetItem *listwidgetitem = new QListWidgetItem(ui->itemListWidget,0);
        listwidgetitem->setSizeHint(QSize(50,50));
        ui->itemListWidget->setItemWidget(listwidgetitem,newWidget);
        ui->itemListWidget->show();
    }

}
void MainWindow::deletItem(){
    QListWidgetItem *listwidgetitem= ui->itemListWidget->currentItem();
    listwidgetitem->setHidden(true);
}

void MainWindow::on_userAvatarPic_clicked()  //未登录按钮点击，登录登录界面
{
    user_login.show();
}
void MainWindow::on_logined_clicked()        //已登录按钮点击，进入登录后的界面
{
    user_login.show();
}

void MainWindow::on_dailyBoardButton_clicked()
{
    if(useraccount == "未登录")
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "请先登录");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_chartAnalysisButton_clicked()
{
    if(useraccount == "未登录")
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "请先登录");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    int i = ui->monthSelector->currentIndex();
    chartsMaking(i+1);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_bugetSettingButton_clicked()
{
    if(useraccount == "未登录")
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "请先登录");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    QSqlQuery query_user;
    query_user.exec("select  id,useraccount from user_login where id >= 0");
    while(query_user.next()){
        useraccount = query_user.value(1).toString();
    //    qDebug()<<useraccount;
    }

    ui->stackedWidget->setCurrentIndex(2);

    QSqlQuery query;
    QDateTime dtm;
    QString timeNow=dtm.currentDateTime().toString("yyyy-MM-dd");
    qDebug()<<timeNow;
        //int year = timeNow.toInt();
        //int day = year % 100;
        //year/=100;
        //int month = year % 100;
        //year/=100;
        int year=timeNow.left(4).toInt();
        qDebug()<<year;
        int thisMonthCost(0),thisDayCost(0),thisYearCost(0);
        QString monthNow = timeNow.remove(7,3);
        //qDebug()<<monthNow;
        timeNow=dtm.currentDateTime().toString("yyyy-MM-dd");
        QString sql =QString("select  money,addtime,useraccount from item where addtime between '%1-%2-%3'and '%4-%5-%6'").arg(year).arg("01").arg("01").arg(year+1).arg("01").arg("00");
        query.exec(sql);
        while(query.next()){
            if (query.value(2).toString() == useraccount) {
            thisYearCost+=query.value(0).toInt();
            if (query.value(1).toString().contains(monthNow,Qt::CaseInsensitive))thisMonthCost+=query.value(0).toInt();
            if (query.value(1).toString().contains(timeNow,Qt::CaseInsensitive)) thisDayCost+=query.value(0).toInt();
            }
        }
    //qDebug()<<thisDayCost;
    //qDebug()<<thisMonthCost;
    //qDebug()<<thisYearCost;

    sql = QString("select dayBudget,monthBudget,yearBudget from user_budget_setting where useraccount = '%1'").arg(useraccount);
    query.exec(sql);
    int dayBudget(0),monthBudget(0),yearBudget(0);
    while(query.next()){
         qDebug()<<query.value(0).toInt();
         qDebug()<<query.value(1).toInt();
         qDebug()<<query.value(2).toInt();
         dayBudget=query.value(0).toInt();
         monthBudget=query.value(1).toInt();
         yearBudget=query.value(2).toInt();
    }
    
    if (thisDayCost>dayBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "今日预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->dayprogressBar->setMaximum(thisDayCost);
    }
    else ui->dayprogressBar->setMaximum(dayBudget);

    if (thisMonthCost>monthBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "本月预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->monthprogressBar->setMaximum(thisMonthCost);
    }
    else ui->monthprogressBar->setMaximum(monthBudget);

    if (thisYearCost>yearBudget)//TODO 增加弹窗提示
    {
        QMessageBox message(QMessageBox::NoIcon, "whoops！", "今年预算超支");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_what the --.png"));
        message.exec();
        ui->yearprogressBar->setMaximum(thisYearCost);
    }
    else ui->yearprogressBar->setMaximum(yearBudget);
    
    ui->dayprogressBar->setValue(thisDayCost);
    ui->monthprogressBar->setValue(thisMonthCost);
    ui->yearprogressBar->setValue(thisYearCost);


    QString daylabel = QString("每日预算：%1/%2").arg(thisDayCost).arg(dayBudget);
    QString monthlabel = QString("每月预算：%1/%2").arg(thisMonthCost).arg(monthBudget);
    QString yearlabel = QString("每年预算：%1/%2").arg(thisYearCost).arg(yearBudget);
    ui->dayBudgetSetlabel->setText(daylabel);
    ui->monthBudgetSetlabel->setText(monthlabel);
    ui->yearBudgetSetlabel->setText(yearlabel);
}

void MainWindow::on_dayBudgetSetButton_clicked()
{
   //接收新的预算
   int dayBudget;
   dayBudget = QInputDialog::getInt(this,"设置","每日预算：",0);
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(dayBudget).arg(0).arg(0).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set dayBudget = '%1' where useraccount = '%2'").arg(dayBudget).arg(useraccount);
        query.exec(sql);
    }
   //更新预算进度标签
   QString daylabel = QString("每日预算：%1/%2").arg(ui->dayprogressBar->value()).arg(dayBudget);
   ui->dayBudgetSetlabel->setText(daylabel);
   if (ui->dayprogressBar->value()>dayBudget)//TODO 增加弹窗提示
   {

       ui->dayprogressBar->setMaximum(ui->dayprogressBar->value());
   }
   else ui->dayprogressBar->setMaximum(dayBudget);
   ui->dayBudgetSetlabel->show();

}

void MainWindow::on_monthBudgetSetButton_clicked()
{
    //接收新的预算
    int monthBudget;
    monthBudget = QInputDialog::getInt(this,"设置","每月预算：",0);
    //TODO添加数据库操作
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(0).arg(monthBudget).arg(0).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set monthBudget = '%1' where useraccount = '%2'").arg(monthBudget).arg(useraccount);
        query.exec(sql);
    }

    //更新预算进度标签
    QString monthlabel = QString("每月预算：%1/%2").arg(ui->monthprogressBar->value()).arg(monthBudget);
    ui->monthBudgetSetlabel->setText(monthlabel);
    if (ui->monthprogressBar->value()>monthBudget)//TODO 增加弹窗提示
    {
        ui->monthprogressBar->setMaximum(ui->monthprogressBar->value());
    }
    else ui->monthprogressBar->setMaximum(monthBudget);

    ui->monthBudgetSetlabel->show();
}

void MainWindow::on_yearBudgetSetButton_clicked()
{
    //接收新的预算
    int yearBudget;
    yearBudget = QInputDialog::getInt(this,"设置","每年预算：",0);
    //TODO添加数据库操作
    QSqlQuery query;
    QSqlQueryModel *model_2 = new QSqlQueryModel;
    QString sql = QString("select count (*) from user_budget_setting where useraccount = '%1'").arg(useraccount);
    model_2->setQuery(sql);
    QModelIndex index1 = model_2->index(0,0);
    if (index1.data()==0){//插入一条新记录
        sql = QString("insert into user_budget_setting(id,dayBudget,monthBudget,yearBudget,useraccount) values(NULL,'%1','%2','%3','%4')").arg(0).arg(0).arg(yearBudget).arg(useraccount);
        qDebug()<<useraccount;
        query.exec(sql);
    }
    else { //更新记录
        //Debug
        qDebug()<<"更新成功";
        sql = QString("update user_budget_setting set yearBudget = '%1' where useraccount = '%2'").arg(yearBudget).arg(useraccount);
        query.exec(sql);
    }
    //更新预算进度标签
    QString yearlabel = QString("每年预算：%1/%2").arg(ui->yearprogressBar->value()).arg(yearBudget);
    ui->yearBudgetSetlabel->setText(yearlabel);
    if (ui->yearprogressBar->value()>yearBudget)//TODO 增加弹窗提示
    {
        ui->yearprogressBar->setMaximum(ui->yearprogressBar->value());
    }
    else ui->yearprogressBar->setMaximum(yearBudget);
    ui->yearBudgetSetlabel->show();
}


void MainWindow::on_monthSelector_activated(int index)
{
    int x = index;
    x++;
    chartsMaking(x);
}

//饼状图生成函数
void MainWindow::chartsMaking(int month)
{
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<"BarMaking:"<<id<<useraccount;
    }//获取当前用户账号
    QSqlQuery query;
    QString sql =QString("select  money, type from item where useraccount = '%1'and addtime between '2023-0%2-%3'and '2023-0%4-%5'").arg(useraccount).arg(month).arg("01").arg(month+1).arg("00") ;
    query.exec(sql);
    int eatMoney,entertainmentMoney,eduMoney,clothesMoney,medicineMoney,travelMoney;
    eatMoney=0;
    entertainmentMoney = 0;
    eduMoney=0;
    clothesMoney=0;
    medicineMoney=0;
    travelMoney=0;
    //int eatMoney_per,entertainmentMoney_per,eduMoney_per;
    while (query.next()) {
        if(query.value(1).toString()=="饮食"){
             eatMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="娱乐"){
             entertainmentMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="教育"){
             eduMoney+=query.value(0).toInt();
        }
        else if(query.value(1).toString()=="服饰"){
             clothesMoney+=query.value(0).toInt();
        }
        else if(query.value(1).toString()=="医疗"){
             medicineMoney+=query.value(0).toInt();
        }
        else if(query.value(1).toString()=="旅行"){
             travelMoney+=query.value(0).toInt();
        }
    }
    qDebug()<<eatMoney;
    qDebug()<<entertainmentMoney;
    qDebug()<<eduMoney;
    qDebug()<<clothesMoney;
    qDebug()<<medicineMoney;
    qDebug()<<travelMoney;
    qDebug()<<month;
    QPieSeries *series = new QPieSeries();
    series->append("饮食",eatMoney);
    series->append("娱乐",entertainmentMoney);
    series->append("教育",eduMoney);
    series->append("服饰",clothesMoney);
    series->append("医疗",medicineMoney);
    series->append("旅行",travelMoney);
    series->setLabelsVisible();
//涂色
    QPieSlice *slice_red = series->slices().at(0);
    QPieSlice *slice_green = series->slices().at(1);
    QPieSlice *slice_blue = series->slices().at(2);
    QPieSlice *slice_yellow = series->slices().at(3);
    QPieSlice *slice_purple = series->slices().at(4);
    QPieSlice *slice_grey = series->slices().at(5);
    slice_red->setColor(QColor(255,0,0,255));
    slice_green->setColor(QColor(0,255,0,255));
    slice_blue->setColor(QColor(0,0,255,255));
    slice_yellow->setColor(QColor(255,255,0));
    slice_purple->setColor(QColor(128,0,128));
    slice_grey->setColor(QColor(128,128,128));

    QChart *chart = new QChart();
    chart->addSeries(series);
    QString title = QString("您的%1月的账单统计图").arg(month);
    chart->setTitle(title);
    chart->legend()->hide();
    ui->chartViewer->setChart(chart);

//    QChartView *chartview = new QChartView(chart);
//    chartview->setRenderHint(QPainter::Antialiasing);
//    chartview->resize(400,400);
//    chartview->show();
//    ui->graphicsView->setChart(chart);
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

//柱状图生成函数
void MainWindow::barChartsMaking(int month){
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        qDebug()<<"BarMaking:"<<id<<useraccount;
    }//获取当前用户账号
    QStringList category;
    QSqlQuery query;
    QString sql =QString("select  money, type from item where useraccount = '%1'and addtime between '2023-0%2-%3'and '2023-0%4-%5'").arg(useraccount).arg(month).arg("01").arg(month+1).arg("00") ;
    query.exec(sql);
    QBarSet *set = new QBarSet(" 总览");
//    QBarSet *setEntertainment = new QBarSet("娱乐");
//    QBarSet *setEdu = new QBarSet("教育");
//    QBarSet *setClothes = new QBarSet("服饰");
//    QBarSet *setMedicine = new QBarSet("医疗");

    int eatMoney,entertainmentMoney,eduMoney,clothesMoney,medicineMoney,travelMoney;
    eatMoney=0;
    entertainmentMoney = 0;
    eduMoney=0;
    clothesMoney=0;
    medicineMoney=0;
    travelMoney=0;
    //int eatMoney_per,entertainmentMoney_per,eduMoney_per;
    while (query.next()) {
        if(query.value(1).toString()=="饮食"){
             eatMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="娱乐"){
             entertainmentMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="教育"){
             eduMoney+=query.value(0).toInt();
        }else if(query.value(1).toString()=="服饰"){
            clothesMoney+=query.value(0).toInt();
       }else if(query.value(1).toString()=="医疗"){
            medicineMoney+=query.value(0).toInt();
       }else if(query.value(1).toString()=="旅行"){
            travelMoney+=query.value(0).toInt();
       }
    }
    *set<<eatMoney<<entertainmentMoney<<eduMoney<<clothesMoney<<medicineMoney<<travelMoney;
//    *setEntertainment<<entertainmentMoney;
//    *setEdu<<eduMoney;
    /*while(query.next()){
        int i=1;
        if(query.value(1).toString()=="饮食"){
            *setEat<<query.value(0).toInt();
            //*setEntertainment<<0;
            //*setEdu<<0;
            category<<"饮食";
        }else if(query.value(1).toString()=="娱乐"){
            //*setEat<<0;
            *setEntertainment<<query.value(0).toInt();
            //*setEdu<<0;
            category<<"娱乐";
        }else if(query.value(1).toString()=="教育"){
            //*setEat<<0;
            //*setEntertainment<<0;
            *setEdu<<query.value(0).toInt();
            category<<"教育";
        }

    }*/
    QString eat=QString("饮食");
    QString entertainment=QString("娱乐");
    QString edu=QString("教育");
    QString clothes=QString("服饰");
    QString medicine=QString("医疗");
    QString travel=QString("旅行");
    category<<"饮食"<<"娱乐"<<"教育"<<"服饰"<<"医疗"<<"旅行";
    QBarSeries *series = new QBarSeries();
    series->append(set);
    //series->append(setEntertainment);
    //series->append(setEdu);
    QChart *chart = new QChart();
    QString title = QString("您的%1月的账单统计图").arg(month);
    chart->setTitle(title);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(eat);
    axisX->append(entertainment);
    axisX->append(edu);
    axisX->append(clothes);
    axisX->append(medicine);
    axisX->append(travel);

    chart->createDefaultAxes();
    chart->addSeries(series);
    chart->setAxisX(axisX,series);
    ui->chartViewer->setChart(chart);
}

void MainWindow::moneyAnalysis(int month)
{
    QSqlQuery query1;
        query1.exec("select  id,useraccount from user_login where id >= 0");
        while (query1.next()) {
            QString id = query1.value(0).toString();
            useraccount = query1.value(1).toString();
            qDebug()<<"BarMaking:"<<id<<useraccount;
        }//获取当前用户账号
        QSqlQuery query;
        QString sql =QString("select  money, type from item where useraccount = '%1'and addtime between '2023-0%2-%3'and '2023-0%4-%5'").arg(useraccount).arg(month).arg("01").arg(month+1).arg("00") ;
        query.exec(sql);
        int eatMoney,entertainmentMoney,eduMoney,clothesMoney,medicineMoney,travelMoney,Money;
            eatMoney=0;
            entertainmentMoney = 0;
            eduMoney=0;
            clothesMoney=0;
            medicineMoney=0;
            travelMoney=0;
            Money=0;
            while (query.next()) {
                if(query.value(1).toString()=="饮食"){
                     eatMoney+=query.value(0).toInt();
                }else if(query.value(1).toString()=="娱乐"){
                     entertainmentMoney+=query.value(0).toInt();
                }else if(query.value(1).toString()=="教育"){
                     eduMoney+=query.value(0).toInt();
                }
                else if(query.value(1).toString()=="服饰"){
                     clothesMoney+=query.value(0).toInt();
                }
                else if(query.value(1).toString()=="医疗"){
                     medicineMoney+=query.value(0).toInt();
                }
                else if(query.value(1).toString()=="旅行"){
                     travelMoney+=query.value(0).toInt();
                }
            }
            Money=eatMoney+eduMoney+entertainmentMoney+medicineMoney+travelMoney+clothesMoney;
            QDateTime dtm;
            QString timeNow=dtm.currentDateTime().toString("yyyy-MM-dd");//当前时间
            int judge=timeNow.right(5).left(2).toInt();
            int n=timeNow.right(2).toInt();
            double averageMoney;
            if(month==judge){averageMoney=Money/n;}
            else{averageMoney=Money/30;}
            int mMoney=qMax(eatMoney,qMax(eduMoney,qMax(clothesMoney,qMax(medicineMoney,qMax(travelMoney,entertainmentMoney)))));
            QString a,b;
            if(mMoney==eatMoney){a=QString("您的最大花销来源于饮食");}
            else if(mMoney==eduMoney){a=QString("您的最大花销来源于教育");}
            else if(mMoney==entertainmentMoney){a=QString("您的最大花销来源于娱乐");}
            else if(mMoney==clothesMoney){a=QString("您的最大花销来源于服饰");}
            else if(mMoney==eduMoney){a=QString("您的最大花销来源于医疗");}
            else{a=QString("您的最大花销来源于旅行");}
            if(entertainmentMoney>=300){b=QString("本月在娱乐上花费过多，请合理消费");}
            QString ans=QString("本月您总共花费了%1元，其中"+a+"，共花费了%2元，平均每天使用%3元。"+b).arg(Money).arg(mMoney).arg(averageMoney);
            if(mMoney!=0) {QMessageBox message(QMessageBox::NoIcon, "账目分析", ans);
            message.exec();}
            else { QMessageBox message(QMessageBox::NoIcon, "账目分析", "本月无花销");
            message.exec();}
}
void MainWindow::on_delet_pushButton_2_clicked()
{
    QListWidgetItem *listwidgetitem= ui->itemListWidget->currentItem();
    listwidgetitem->setHidden(true);
    QSqlQuery query;
    int curRow = ui->itemListWidget->currentIndex().row();     //鼠标选择删除第几行
        QModelIndex index = ui->itemListWidget->currentIndex();
        int id=index.sibling(curRow,0).data().toInt();
        QString str =QString("delete from item where id = '%1'").arg(curRow) ;
        query.exec(str);
}
//编辑按钮
void MainWindow::on_edit_pushButton_clicked()
{
    QSqlQuery query;
    query.exec("select id,money,type,content,addtime from item where id >= 1");
    QString str;
    QString str1;
    QString str2;
    QString str3;
    QString str4;
    QString str5;
    QDateTime dtm;
    int dtm0;
    while(query.next())
    {
        //query.value(0)是id的值，将其转换为int型

        //QString str0 = dtm.toString("yyyy-mm-dd hh:mm:ss");
        str1 = query.value(0).toString();
        str2 = query.value(1).toString();
        str3 = query.value(2).toString();
        str4 = query.value(3).toString();
        str5 = query.value(4).toString();
        //输出两个值
        qDebug() << str1<< str2<< str3<< str4<<str5 ;
    }
}

void MainWindow::on_clearButton_clicked()
{
    QSqlQuery query;
    query.exec("drop table item");
    query.exec("drop table user");
    query.exec("drop table user_login");
    query.exec("drop table user_buget_setting");
    query.exec("create table item(id INTEGER PRIMARY KEY AUTOINCREMENT,money int,type varchar,content varchar,addtime time,useraccount varchar)");
    query.exec("create table user(id INTEGER PRIMARY KEY AUTOINCREMENT,useraccount varchar,userpassword varchar,protect varchar,protectPass varchar)");
    query.exec("create table user_login(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar)");
    query.exec("create table user_budget_setting(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar,dayBudget int,monthBudget int,yearBudget int)");
    ui->itemListWidget->clear();
}
void MainWindow::user_login_sucess(){     //登录成功

    ui->userAvatarPic->hide(); //未登录按钮隐藏
    ui->logined->show();       //已登录按钮显示
    QSqlQuery query1;
    query1.exec("select  id,useraccount from user_login where id >= 0");
    while (query1.next()) {
        QString id = query1.value(0).toString();
        useraccount = query1.value(1).toString();
        //qDebug()<<"mainwindow:"<<id<<useraccount;
    }
    qDebug()<<useraccount;
    QString sql = QString("select id,money,type,content,addtime,useraccount from item where useraccount = '%1'").arg(useraccount);
    query1.exec(sql);
    ui->itemListWidget->clear();
    while (query1.next()) {
        QString id = query1.value(0).toString();
        int money = query1.value(1).toInt();
        QString type = query1.value(2).toString();
        QString content = query1.value(3).toString();
        QString time=query1.value(4).toString();
        useraccount = query1.value(5).toString();


        newWidget = new itemWidget(money,type,content,time,ui->itemListWidget);
        QListWidgetItem *listwidgetitem = new QListWidgetItem(ui->itemListWidget,0);
        listwidgetitem->setSizeHint(QSize(50,50));
        ui->itemListWidget->setItemWidget(listwidgetitem,newWidget);
        ui->itemListWidget->show();
        //qDebug()<<money<<type<<content<<useraccount;
    }
}


void MainWindow::exit_Login()   //用户退出登录响应函数
{
    ui->itemListWidget->clear();
    ui->userAvatarPic->show(); //未登录按钮隐藏
    ui->logined->hide();       //已登录按钮显示
    useraccount = "未登录";
}

void MainWindow::on_selectPieChart_clicked()
{
    int index;
    index = ui->monthSelector->currentIndex();
    chartsMaking(index+1);
}

void MainWindow::on_selectLinerChart_clicked()
{
    int index;
    index = ui->monthSelector->currentIndex();
    barChartsMaking(index+1);
}

void MainWindow::on_analysis_clicked()
{
    int index;
    index = ui->monthSelector->currentIndex();
    moneyAnalysis(index+1);
}
void MainWindow::on_selectLinerChart_pressed()
{
    ui->selectLinerChart->setStyleSheet("QPushButton {"
                                     "  background: rgb(169, 169, 169);"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
    ui->selectPieChart->setStyleSheet("QPushButton {"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
}


void MainWindow::on_selectPieChart_pressed()
{
    ui->selectPieChart->setStyleSheet("QPushButton {"
                                     "  background: rgb(169, 169, 169);"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
    ui->selectLinerChart->setStyleSheet("QPushButton {"
                                     "   border: 1px solid #8f8f91;"
                                     "   border-radius: 6px;"
                                    "}");
}

void MainWindow::on_find_clicked()          //界面上放大镜图标得点击响应函数
{
    if(useraccount == "未登录")
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "请先登录");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    ui->findout->setText("");
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_clicked()     //查询按钮点击响应时间
{

    QString res;
    QDateTime startTime = ui->dateTimeEdit->dateTime();  //开始日期时间
    QDateTime endTime = ui->dateTimeEdit_3->dateTime();  //结束日期时间
    //QString sql = QString("select id,money,type,content,addtime from item where useraccount = '%1' and addtime between '%2'and '%3'").arg(useraccount).arg(startTime.toString()).arg(endTime.toString());
    QString sql = QString("select money,type,content,addtime from item where useraccount = '%1'").arg(useraccount);
    QSqlQuery query;
    query.exec(sql);
     while (query.next())
     {
         QDateTime time;
         time = QDateTime::fromString(query.value(3).toString(), "yyyy-MM-dd hh:mm:ss");
         int curtime = time.toTime_t();
         int s = startTime.toTime_t();
         int e = endTime.toTime_t();
         if(curtime >= s && curtime <= e)
         {
             //qDebug() << "123" << endl;
             res += query.value(0).toString()+ "\t\t"  + query.value(1).toString()+ "\t\t"  + query.value(2).toString()+ "\t\t"  + query.value(3).toString() + "\n";
         }
      }
     ui->findout->setText(res);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


