#include "itemmark.h"
#include "ui_itemmark.h"
#include <QString>
#include <QDateTime>
itemMark::itemMark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemMark)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette p = this->palette();
    QPixmap pix(":/img_asset/icon_asset/background_right.png");
    p.setBrush(QPalette::Window, QBrush(pix));
    this->setPalette(p);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&itemMark::close);
    connect(ui->confirm_btn,&QPushButton::clicked,
            [=](){
        int moneyNumber = ui->moneyLine->text().toInt();
        QString typeName = ui->Type_comboBox->currentText();
        QString allOfContent = ui->content->toPlainText();
        QDateTime dtm;//当前的时间
        QString time= dtm.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        emit conveyData(moneyNumber,typeName,allOfContent,time);//保存到数据库中后再主界面上分别列出金额、类型、备注和当前时间
        this->hide();
        ui->moneyLine->clear();
        //ui->comboBox->clear();
        ui->content->clear();
    }
            );
}

itemMark::~itemMark()
{
    delete ui;
}
