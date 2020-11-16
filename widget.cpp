#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //初始化网络通信和语音获取参数
    initNetwork();
    UserInfo = new QSettings("UserInofrmation.ini",QSettings::IniFormat);
    ui->textBrowser_UserInfo->setReadOnly(true);
    if(UserInfo->value("OnExists").toBool())
    {
        ui->lineEdit->setText(UserInfo->value("UserName").toString());
    }
    RefuseUserList = new QTimer(this);
    connect(RefuseUserList,SIGNAL(timeout()),SLOT(ReSendBoardCast()));
    ui->widget_2->hide();
}

Widget::~Widget()
{
    delete ui;
//    tcpSocket->quit();
    udpSocket->quit();
    audio_input->quit();
    audio_output->quit();
}

void Widget::on_pushButton_Enter_clicked()
{
    RefuseUserList->stop();
    if(UserInfo->value("OnExists").toBool())
    {
        UserInfo->setValue("UserName",ui->lineEdit->text());
    }
    else
    {
        qsrand(QDateTime::currentSecsSinceEpoch());
        qsrand(qrand());
        UserInfo->setValue("Id",qrand()%99999);
        UserInfo->setValue("OnExists",true);
        UserInfo->setValue("UserName",ui->lineEdit->text());
    }
    int id = UserInfo->value("Id").toInt();
    udpSocket->selfId = id;
    audio_input->UserId  = id;
    ui->textBrowser_UserInfo->setText(QString("用户名：%1").arg(UserInfo->value("UserName").toString())+"\r\n"+QString("用户Id：%1").arg(UserInfo->value("Id").toString()));
    ui->widget->hide();
    ui->widget_2->show();
    audio_output->start();    
    RefuseUserList->start(1000);
    UdpSendBoardCast(UserInfo->value("UserName").toString(),UserInfo->value("Id").toInt());
}

void Widget::on_pushButton_Exit_clicked()
{
    this->close();
}

void Widget::on_checkBox_Mic_clicked(bool checked)
{
    if(checked)
    {
        audio_input->start();
    }else{
        audio_input->quit();
    }
}


