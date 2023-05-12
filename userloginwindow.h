#ifndef USERLOGINWINDOW_H
#define USERLOGINWINDOW_H

#include <QWidget>

namespace Ui {
class userLoginWindow;
}

class userLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit userLoginWindow(QWidget *parent = 0);
    ~userLoginWindow();
signals:
    void loginSuccess();//成功登陆的信号，传递给主窗口
    void exitLogin();   //退出登录的信号
private slots:
    void on_registerButton_clicked();

    void on_return_button_clicked();

    void regUser();

    void sendToDateBase_regist();

    void sendToDateBase_log();

    void on_forgetPas_clicked();  //忘记密码按钮的点击事件

    //void on_forOK_clicked();

    void on_pushButton_3_clicked();

    void on_lookpro_clicked();

    void on_forOK_clicked(bool checked);


    void on_exitLogin_clicked();


private:
    Ui::userLoginWindow *ui;

};

#endif // USERLOGINWINDOW_H
