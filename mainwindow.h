#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<addnewitem.h>
#include<QPushButton>
#include<QLabel>
#include<QPushButton>
#include"userloginwindow.h"
#include<QHBoxLayout>
#include<iostream>
#include<QtCharts/QtCharts>
#include"itemmark.h"
#include"itemwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addNewItemLabel(int money ,QString type,QString content,QString time);
    void deletItem();

    void chartsMaking(int);

    void barChartsMaking(int);

    void moneyAnalysis(int);
private slots:
    void on_addItemButton_clicked();
    void on_userAvatarPic_clicked();

    void on_dailyBoardButton_clicked();

    void on_chartAnalysisButton_clicked();

    void on_bugetSettingButton_clicked();

    void on_dayBudgetSetButton_clicked();

    void on_monthBudgetSetButton_clicked();

    void on_yearBudgetSetButton_clicked();

    void on_monthSelector_activated(int index);

    void on_delet_pushButton_2_clicked();

    void on_edit_pushButton_clicked();

    void on_clearButton_clicked();

    void user_login_sucess();
    void exit_Login();

    void on_selectPieChart_clicked();

    void on_selectLinerChart_clicked();


    void on_selectLinerChart_pressed();

    void on_selectPieChart_pressed();

    void on_find_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_analysis_clicked();

    void on_selectLinerChart_clicked(bool checked);

    void on_chartViewer_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void on_logined_clicked();

private:
    Ui::MainWindow *ui;
    userLoginWindow user_login;
    itemMark itemMark;
    addNewItem newitem;     //弹出的子窗口
    QLabel *newItem;        //添加标签
    QPushButton testButton; //测试按钮，无意义
    QHBoxLayout tagWidgetLayout;
    itemWidget *newWidget;  //list的条目控件
    QString useraccount = "未登录";
};

#endif // MAINWINDOW_H
