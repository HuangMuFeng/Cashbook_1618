#include "userloginwindow.h"
#include "ui_userloginwindow.h"
#include<QFile>
#include<QString>
#include<QDialog>
#include<QFileDialog>
#include<QDebug>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<iostream>
#include<QMessageBox>

using namespace std;


userLoginWindow::userLoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userLoginWindow)
{
    ui->setupUi(this); this->setWindowTitle(" ");

    //注册页面
    ui->userID_reg->setPlaceholderText("请输入用户名");

    ui->userPassword_reg->setPlaceholderText("密码");
    ui->userPassword_reg_2->setPlaceholderText("确认密码");
    ui->userPassword_reg->setEchoMode(QLineEdit::Password);
    ui->userPassword_reg_2->setEchoMode(QLineEdit::Password);

    //ui->userMail->setPlaceholderText("xxx@xxx.xxx");
    //注册操作
    connect(ui->reg_button,&QPushButton::clicked,this,&userLoginWindow::sendToDateBase_regist);
    //登陆操作
    connect(ui->loginButton,&QPushButton::clicked,this,&userLoginWindow::sendToDateBase_log);
    //登录界面
    ui->username->setPlaceholderText("请输入用户名");
    ui->userPassword->setEchoMode(QLineEdit::Password);
    ui->userPassword->setPlaceholderText("请输入密码");

    //注册界面
    ui->protect->setPlaceholderText("可选");
    ui->protect_2->setPlaceholderText("可选");
    //按钮密码按钮文本颜色设置
    QPalette pal = ui->forgetPas->palette();
    pal.setColor(QPalette::ButtonText, Qt::white);
    ui->forgetPas->setPalette(pal);


    //忘记密码按钮设置界面
    ui->forgetUser->setPlaceholderText("请输入你的用户名");
    ui->pro->setPlaceholderText("请输入密保答案,若未设置请忽略");
    QPalette pal2 = ui->lookpro->palette();
    pal2.setColor(QPalette::ButtonText, Qt::white);
    ui->lookpro->setPalette(pal);

    ui->log->show();
}

userLoginWindow::~userLoginWindow()
{
    delete ui;
}


void userLoginWindow::on_registerButton_clicked()
{
    ui->log->hide();
    ui->reg->show();
}

void userLoginWindow::on_return_button_clicked()
{
    ui->log->show();
    ui->reg->hide();
}
void userLoginWindow::sendToDateBase_regist(){     //注册功能
    QSqlQuery query;
    QString sql;
    QString str1 ,str2, str3,str4,str5;
    str1 = ui->userID_reg->text();
    str2 = ui->userPassword_reg->text();
    str3 = ui->userPassword_reg_2->text();
    str4 = ui->protect->text();
    str5 = ui->protect_2->text();

    if(str1.length() == 0 || str2.length() == 0)
    {
        QMessageBox message(QMessageBox::NoIcon, "error", "用户名或密码不可为空");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
        return;
    }
    if (str2 != str3){
        QMessageBox message(QMessageBox::NoIcon, "error", "请确认密码");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();

        ui->userID_reg->clear();
        ui->userPassword_reg->clear();
        ui->userPassword_reg_2->clear();
        //ui->userMail->clear();
        return ;
    }

    QSqlQueryModel *model1=new QSqlQueryModel;
    sql = QString("select count (*)  from user where useraccount = '%1'").arg(str1);
    model1->setQuery(sql);
    QModelIndex index1=model1->index(0,0);
    //query.exec(sql);
    if(index1.data()==0){
        //如果这条数据不存在那么插入这条数据
        sql = QString("insert into user(id, useraccount, userpassword,protect,protectPass) values(NULL,'%1', '%2', '%3','%4')").arg(str1).arg(str2).arg(str4).arg(str5);
        query.exec(sql);
        //debug测试
        query.exec("select id,useraccount,userpassword,useremail from user where id >= 1");
        while(query.next())
        {
            //query.value(0)是id的值，将其转换为int型
            //QString str0 = dtm.toString("yyyy-mm-dd hh:mm:ss");
            str1 = query.value(1).toString();
            str2 = query.value(2).toString();
            str3 = query.value(3).toString();
            //输出两个值
            qDebug() << str1<< str2<< str3 ;
        }
        QMessageBox message(QMessageBox::NoIcon, "", "注册成功");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_laughter.png"));
        message.exec();
        qDebug()<<"注册成功";
        ui->userID_reg->clear();
        ui->userPassword_reg->clear();
        ui->userPassword_reg_2->clear();
        ui->protect->clear();
        ui->protect_2->clear();
        ui->reg->hide();
        ui->log->show();
    }else{
        qDebug()<<"用户已存在";
        QMessageBox message(QMessageBox::NoIcon, "error", "用户名已存在");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
    }
}
void userLoginWindow::sendToDateBase_log(){
    QSqlQuery query;
    QString sql;
    QString str1 ,str2, str3;
    str1 = ui->username->text();//用户输入的用户名
    str2 = ui->userPassword->text();//用户输入的密码
    QSqlQueryModel *model1=new QSqlQueryModel;
    QSqlQueryModel *model2=new QSqlQueryModel;
    sql = QString("select count (*)  from user where useraccount = '%1'").arg(str1);
    model1->setQuery(sql);
    QModelIndex index1=model1->index(0,0);
    if(index1.data()==0){
        QMessageBox message(QMessageBox::NoIcon, "error", "用户名不存在");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
    }else{
        //获取该用户名下的密码进行对比
        sql=QString("select userpassword  from user where useraccount = '%1'").arg(str1);
        model2->setQuery(sql);
        QModelIndex index2=model2->index(0,0);
        if(index2.data()==str2){
            query.exec("create table user_login(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar)");
            sql = QString("insert into user_login(id,useraccount) values(NULL,'%1')").arg(str1);
            query.exec(sql);
            qDebug()<<"userlogwindow:"<<str1;
            emit loginSuccess();//将用户名作为信息传递出去
            //清空输入栏
            ui->username->clear();
            ui->userPassword->clear();

            QMessageBox message(QMessageBox::NoIcon, "Title", "登录成功！");
            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_flag-1.png"));
            message.exec();


            //下次显示imoPage界面
            ui->stackedWidget->setCurrentIndex(3);

            //设置标签内容

            QFont font;
            font.setPointSize(11); // 设置字号
            ui->name->setFont(font);  // 设置字体

            QPalette pe;
            pe.setColor(QPalette::WindowText,Qt::green);
            ui->name->setPalette(pe);

            ui->name->setText("用户" + str1);

            ui->name->setAlignment(Qt::AlignCenter);

            this->close();
            qDebug()<<"登陆成功";
        }else{

            QMessageBox message(QMessageBox::NoIcon, "注意", "密码错误！");
            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_delete-user-rd.png"));
            message.exec();
            qDebug()<<"密码错误";
        }
    }
}

void userLoginWindow::regUser()
{
        int user_num_int;
        qint64 pos;
        QFile file("userAccout.txt");
        bool isOk = file.open(QIODevice::ReadWrite|QIODevice::Text);
        if (isOk == true){
            pos=file.size();
            QString user_num = file.readLine();
            QTextStream txtout(&file);
            for (int i=0;i<user_num_int*4;i++){ if (file.canReadLine()) file.readLine();}
            txtout<<"------"<<endl;
            txtout<<ui->userID_reg->text()<<endl;
            txtout<<ui->userPassword_reg->text()<<endl;
            txtout<<ui->protect->text()<<endl;
            txtout<<ui->protect_2->text()<<endl;
            user_num_int = user_num.toInt() + 1;
            file.close();
         }
         else qDebug()<<"Open Failed";

        isOk = file.open(QIODevice::WriteOnly|QIODevice::Text);
         if (isOk == true)
         {
              QTextStream txtout(&file);
              txtout<<user_num_int<<endl;
         }

         ui->log->show();
         ui->reg->hide();

         ui->userID_reg->clear();
         ui->userPassword_reg->clear();
         ui->userPassword_reg_2->clear();
}


void userLoginWindow::on_forgetPas_clicked()     //忘记密码按钮点击事件
{
    ui->log->hide();
    ui->forgetPas_2->show();
}

void userLoginWindow::on_forOK_clicked(bool checked)         //忘记密码获取用户密码
{
    QSqlQuery query;
    QString sql;
    QString userID = ui->forgetUser->text();//获取输入的用户名
    QString proPas = ui->pro->text();       //获取输入的密保密码
    QSqlQueryModel *model1=new QSqlQueryModel;
    QSqlQueryModel *model2=new QSqlQueryModel;
    sql = QString("select count (*)  from user where useraccount = '%1'").arg(userID);
    model1->setQuery(sql);
    QModelIndex index1=model1->index(0,0);
    if(index1.data()!=0){
        if(checked == true)                      //说明“查看密保问题”按钮被点击
        {
            sql=QString("select protect  from user where useraccount = '%1'").arg(userID);
            model2->setQuery(sql);
            QModelIndex index2=model2->index(0,0);
            QMessageBox message(QMessageBox::NoIcon, "attention", "您的密保问题为:" + index2.data().toString());
            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
            message.exec();
            return;
        }

        //查看密保问题按钮没有被点击

        //检查密保问题是否正确
        sql=QString("select protectPass  from user where useraccount = '%1'").arg(userID);
        model2->setQuery(sql);
        QModelIndex truePas=model2->index(0,0);

        if(proPas == truePas.data().toString())
        {
            sql=QString("select userpassword  from user where useraccount = '%1'").arg(userID);
            model2->setQuery(sql);
            QModelIndex index2=model2->index(0,0);
            QMessageBox message(QMessageBox::NoIcon, "attention", "您的密码为:" + index2.data().toString());
            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
            message.exec();
            ui->forgetUser->clear();
            ui->pro->clear();
            ui->forgetPas_2->hide();
            ui->log->show();
        }
        else
        {
            QMessageBox message(QMessageBox::NoIcon, "error", "密保问题回答错误");
            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
            message.exec();
            ui->pro->clear();
        }
        return;
    }
    else
    {
        ui->forgetUser->clear();
        QMessageBox message(QMessageBox::NoIcon, "error", "用户名不存在");
        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
        message.exec();
    }
}










void userLoginWindow::on_pushButton_3_clicked()//忘记密码界面取消按钮点击响应函数
{
    ui->forgetUser->clear();
    ui->pro->clear();
    ui->forgetPas_2->hide();
    ui->log->show();
}


void userLoginWindow::on_lookpro_clicked()  //查看密保按钮的点击响应函数
{
    on_forOK_clicked(true);
}


void userLoginWindow::on_exitLogin_clicked()//退出登录按钮点击
{
    ui->stackedWidget->setCurrentIndex(0);
    emit exitLogin();
}


















//#include "userloginwindow.h"
//#include "ui_userloginwindow.h"
//#include<QFile>
//#include<QString>
//#include<QDialog>
//#include<QFileDialog>
//#include<QDebug>
//#include<QSqlQuery>
//#include<QSqlQueryModel>
//#include<iostream>
//#include<QMessageBox>

//using namespace std;


//userLoginWindow::userLoginWindow(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::userLoginWindow)
//{
//    ui->setupUi(this); this->setWindowTitle(" ");

//    //注册页面
//    ui->userID_reg->setPlaceholderText("请输入用户名");

//    ui->userPassword_reg->setPlaceholderText("密码");
//    ui->userPassword_reg_2->setPlaceholderText("确认密码");
//    ui->userPassword_reg->setEchoMode(QLineEdit::Password);
//    ui->userPassword_reg_2->setEchoMode(QLineEdit::Password);

//    //ui->userMail->setPlaceholderText("xxx@xxx.xxx");
//    //注册操作
//    connect(ui->reg_button,&QPushButton::clicked,this,&userLoginWindow::sendToDateBase_regist);
//    //登陆操作
//    connect(ui->loginButton,&QPushButton::clicked,this,&userLoginWindow::sendToDateBase_log);
//    //登录界面
//    ui->username->setPlaceholderText("请输入用户名");
//    ui->userPassword->setEchoMode(QLineEdit::Password);
//    ui->userPassword->setPlaceholderText("请输入密码");

//    //按钮密码按钮文本颜色设置
//    QPalette pal = ui->forgetPas->palette();
//    pal.setColor(QPalette::ButtonText, Qt::white);
//    ui->forgetPas->setPalette(pal);


//    //忘记密码按钮设置界面
//    ui->forgetUser->setPlaceholderText("请输入你的用户名");



//    ui->log->show();
//}

//userLoginWindow::~userLoginWindow()
//{
//    delete ui;
//}


//void userLoginWindow::on_registerButton_clicked()
//{
//    ui->log->hide();
//    ui->reg->show();
//}

//void userLoginWindow::on_return_button_clicked()
//{
//    ui->log->show();
//    ui->reg->hide();
//}
//void userLoginWindow::sendToDateBase_regist(){
//    QSqlQuery query;
//    QString sql;
//    QString str1 ,str2, str3,str4;
//    str1 = ui->userID_reg->text();
//    str2 = ui->userPassword_reg->text();
//    //str3 = ui->userMail->text();
//    str4 = ui->userPassword_reg_2->text();
//    if (str2 != str4 ){
//        QMessageBox message(QMessageBox::NoIcon, "error", "请确认密码");
//        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
//        message.exec();

//        ui->userID_reg->clear();
//        ui->userPassword_reg->clear();
//        ui->userPassword_reg_2->clear();
//        //ui->userMail->clear();
//        return ;
//    }

//    QSqlQueryModel *model1=new QSqlQueryModel;
//    sql = QString("select count (*)  from user where useraccount = '%1'").arg(str1);
//    model1->setQuery(sql);
//    QModelIndex index1=model1->index(0,0);
//    //query.exec(sql);
//    if(index1.data()==0){
//        //如果这条数据不存在那么插入这条数据
//        sql = QString("insert into user(id, useraccount, userpassword,useremail) values(NULL,'%1', '%2', '%3')").arg(str1).arg(str2).arg(str3);
//        query.exec(sql);
//        //debug测试
//        query.exec("select id,useraccount,userpassword,useremail from user where id >= 1");
//        while(query.next())
//        {
//            //query.value(0)是id的值，将其转换为int型
//            //QString str0 = dtm.toString("yyyy-mm-dd hh:mm:ss");
//            str1 = query.value(1).toString();
//            str2 = query.value(2).toString();
//            str3 = query.value(3).toString();
//            //输出两个值
//            qDebug() << str1<< str2<< str3 ;
//        }
//        QMessageBox message(QMessageBox::NoIcon, "", "注册成功");
//        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_laughter.png"));
//        message.exec();
//        qDebug()<<"注册成功";
//        ui->userID_reg->clear();
//        ui->userPassword_reg->clear();
//        ui->userPassword_reg_2->clear();
//        //ui->userMail->clear();
//        ui->reg->hide();
//        ui->log->show();
//    }else{
//        qDebug()<<"用户已存在";
//        QMessageBox message(QMessageBox::NoIcon, "error", "用户名已存在");
//        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
//        message.exec();
//    }
//}
//void userLoginWindow::sendToDateBase_log(){
//    QSqlQuery query;
//    QString sql;
//    QString str1 ,str2, str3;
//    str1 = ui->username->text();//用户输入的用户名
//    str2 = ui->userPassword->text();//用户输入的密码
//    QSqlQueryModel *model1=new QSqlQueryModel;
//    QSqlQueryModel *model2=new QSqlQueryModel;
//    sql = QString("select count (*)  from user where useraccount = '%1'").arg(str1);
//    model1->setQuery(sql);
//    QModelIndex index1=model1->index(0,0);
//    if(index1.data()==0){
//        qDebug()<<"用户名不存在";
//    }else{
//        //获取该用户名下的密码进行对比
//        sql=QString("select userpassword  from user where useraccount = '%1'").arg(str1);
//        model2->setQuery(sql);
//        QModelIndex index2=model2->index(0,0);
//        if(index2.data()==str2){
//            query.exec("create table user_login(id INTEGER PRIMARY KEY AUTOINCREMENT ,useraccount varchar)");
//            sql = QString("insert into user_login(id,useraccount) values(NULL,'%1')").arg(str1);
//            query.exec(sql);
//            qDebug()<<"userlogwindow:"<<str1;
//            emit loginSuccess();//将用户名作为信息传递出去
//            //清空输入栏
//            ui->username->clear();
//            ui->userPassword->clear();

//            QMessageBox message(QMessageBox::NoIcon, "Title", "登录成功！");
//            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_flag-1.png"));
//            message.exec();

//            this->close();
//            qDebug()<<"登陆成功";
//        }else{

//            QMessageBox message(QMessageBox::NoIcon, "注意", "密码错误！");
//            message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_delete-user-rd.png"));
//            message.exec();
//            qDebug()<<"密码错误";
//        }
//    }
//}

//void userLoginWindow::regUser()
//{
//        int user_num_int;
//        qint64 pos;
//        QFile file("userAccout.txt");
//        bool isOk = file.open(QIODevice::ReadWrite|QIODevice::Text);
//        if (isOk == true){
//            pos=file.size();
//            QString user_num = file.readLine();
//            QTextStream txtout(&file);
//            for (int i=0;i<user_num_int*4;i++){ if (file.canReadLine()) file.readLine();}
//            txtout<<"------"<<endl;
//            txtout<<ui->userID_reg->text()<<endl;
//            txtout<<ui->userPassword_reg->text()<<endl;
//            //txtout<<ui->userMail->text()<<endl;
//            user_num_int = user_num.toInt() + 1;
//            file.close();
//         }
//         else qDebug()<<"Open Failed";

//        isOk = file.open(QIODevice::WriteOnly|QIODevice::Text);
//         if (isOk == true)
//         {
//              QTextStream txtout(&file);
//              txtout<<user_num_int<<endl;
//         }

//         ui->log->show();
//         ui->reg->hide();

//         ui->userID_reg->clear();
//         ui->userPassword_reg->clear();
//         ui->userPassword_reg_2->clear();
//}


//void userLoginWindow::on_forgetPas_clicked()     //忘记密码按钮点击事件
//{
//    ui->log->hide();
//    ui->forgetPas_2->show();
//}

//void userLoginWindow::on_forOK_clicked()          //获取用户密码
//{
//    QSqlQuery query;
//    QString sql;
//    QString userID = ui->forgetUser->text();//获取输入的用户名
//    QSqlQueryModel *model1=new QSqlQueryModel;
//    QSqlQueryModel *model2=new QSqlQueryModel;
//    sql = QString("select count (*)  from user where useraccount = '%1'").arg(userID);
//    model1->setQuery(sql);
//    QModelIndex index1=model1->index(0,0);
//    if(index1.data()!=0){
//        sql=QString("select userpassword  from user where useraccount = '%1'").arg(userID);
//        model2->setQuery(sql);
//        QModelIndex index2=model2->index(0,0);
//        QMessageBox message(QMessageBox::NoIcon, "attention", "您的密码为:" + index2.data().toString());
//        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
//        message.exec();
//        ui->forgetPas_2->hide();
//        ui->log->show();
//        return;
//    }
//    else
//    {
//        QMessageBox message(QMessageBox::NoIcon, "error", "用户名不存在");
//        message.setIconPixmap(QPixmap(":/img_asset/icon_asset/2x_surprised.png"));
//        message.exec();
//    }
//}










//void userLoginWindow::on_pushButton_3_clicked()//忘记密码界面取消按钮点击响应函数
//{
//    ui->forgetPas_2->hide();
//    ui->log->show();
//}












