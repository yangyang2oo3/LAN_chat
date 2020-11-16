#include "widget.h"
#include "ui_widget.h"

void Widget::initNetwork()
{
    udpSocket = new UdpSocket;
    audio_input = new Audio_input;
    audio_output = new Audio_output;

    connect(this,SIGNAL(udpSend(QByteArray)),udpSocket,SLOT(SendDataUdp(QByteArray)));

    connect(udpSocket,SIGNAL(AudioFromUdp(char*,int)),SLOT(ReceiveAudioData(char*,int)));

    connect(audio_input,SIGNAL(AudioInput(QByteArray)),SLOT(get_Audio_input(QByteArray)));

    connect(this,SIGNAL(AudioReceived(char*)),audio_output,SLOT(OnAudiooutputting(char*)));

    connect(udpSocket,SIGNAL(ReceiveBroadCast(QString,int,QByteArray,QByteArray)),this,SLOT(ReceiveBroadCast(QString,int,QByteArray,QByteArray)));

    connect(this,SIGNAL(sendBroadCast(QString,int)),udpSocket,SLOT(SendBroadCast(QString,int)));


    /*
    //useless:程序中无TCP需求，使用Udp取代
    //    tcpSocket = new TcpSocket();
    //    connect(this,SIGNAL(tcpSend(QByteArray)),tcpSocket,SLOT(sendDataTcp(QByteArray)));
    //    connect(tcpSocket,SIGNAL(MessageFromTcp(QByteArray)),SLOT(ReceiveTcpData(QByteArray)));
    //    connect(this,SIGNAL(setIpAndPort(QString,int)),tcpSocket,SLOT(setPortAndIp(QString,int)));
    //    tcpSocket->start();
*/
    udpSocket->start();

}

void Widget::ReSendBoardCast()
{
    UdpSendBoardCast(UserInfo->value("UserName").toString(),UserInfo->value("Id").toInt());
    SpeakingFlag=0;
}

//接收广播信号
void Widget::ReceiveBroadCast(QString Name,int Id,QByteArray address,QByteArray Adata)
{
    bool notOnExists = true;
    User userTemp;
    userTemp.Id = Id;
    userTemp.UserName = Name;
    userTemp.Adata = Adata;
    userTemp.Ip = address;
    for(int i=0;i<UserList.length();i++)
    {
        if(UserList.at(i).Id==userTemp.Id)
        {
            notOnExists = false;
        }
    }
    if(notOnExists)
    {
        UserList.append(userTemp);
    }
    ui->listWidget_ALL_user->clear();
    for(int i=0;i<UserList.length();i++)
    {
        //qDebug()<<UserList.at(i).UserName;
        ui->listWidget_ALL_user->addItem(QString(UserList.at(i).UserName).append(QString("  ID:%1").arg(UserList.at(i).Id)).append("   IP:").append(UserList.at(i).Ip));
    }
    //TODO:实现接收广播后判断用户是否在线功能
}

//发送广播信号
void Widget::UdpSendBoardCast(QString Name, int Id)
{
    emit sendBroadCast(Name,Id);
}

//设置连接地址和端口
//void Widget::SetConnectInfo(QHostAddress ip, int port)
//{
//    emit setIpAndPort(ip,port);
//}

//通过UDP发送数据
void Widget::UdpSendData(QByteArray send)
{
    emit udpSend(send);

//    qDebug()<<send.length();
}
/*
//useless:无需TCP传输数据
//接收到TCP数据
//void Widget::ReceiveTcpData(QByteArray data)
//{
//    DataFromTcp = data;
//}
//通过TCP发送数据
//void Widget::TcpSendData(QByteArray send)
//{
//    emit tcpSend(send);
//}
*/
//接收到UDP数据
void Widget::ReceiveAudioData(char* data,int SpeakerId)
{
//    DataFromUdp = data;
    emit AudioReceived(data);//播放
    for(int i=0;i<UserList.length();i++)
    {
        if(UserList.at(i).Id==SpeakerId)
        {
            if(SpeakingFlag<1)
            {
                ui->listWidget_ALL_user->item(i)->setText(ui->listWidget_ALL_user->item(i)->text().append(" Speaking"));

            }
        }
    }
    SpeakingFlag++;
}
//获取音频数据
void Widget::get_Audio_input(QByteArray Audio)
{
    //AudioInput.append(Audio);//记录音频数据
    UdpSendData(Audio);//通过Udp发送
}
