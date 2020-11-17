#include "widget.h"
#include "ui_widget.h"

/*---这是用于连接各个信号与槽还有进行一些处理的文件---*/

void Widget::initNetwork()
{
    //给需要使用的类或线程分配内存
    udpSocket = new UdpSocket;
    audio_input = new Audio_input;
    audio_output = new Audio_output;
    //连接各个信号与槽使获取输入时或发送时可调用目标函数
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
    //UdpSocket时继承于QThread的类，是一个用于使用UDP功能的线程，在程序开始时启动以便调用
    udpSocket->start();

}

//这是一个由定时器触发的槽函数，用于发送UDP广播包
void Widget::ReSendBoardCast()
{
    //向广播包添加内容 用户名 用户Id
    UdpSendBoardCast(UserInfo->value("UserName").toString(),UserInfo->value("Id").toInt());
    //将正在说话标志位重置
    SpeakingFlag=0;
}

//接收UDP广播信号
void Widget::ReceiveBroadCast(QString Name,int Id,QByteArray address,QByteArray Adata)
{
    //声明一个标志用户是否上过线的布尔变量，默认为未在线
    bool notOnExists = true;
    //User是自定义结构体，见UdpSocket.h
    User userTemp;
    //获取用户Id
    userTemp.Id = Id;
    //获取用户名
    userTemp.UserName = Name;
    //获取附带的内容
    userTemp.Adata = Adata;
    //获取网络地址
    userTemp.Ip = address;
    //循环遍历用户表
    for(int i=0;i<UserList.length();i++)
    {
        //当在用户表中找到这个用户
        if(UserList.at(i).Id==userTemp.Id)
        {
            //标记为已在线
            notOnExists = false;
        }
    }
    //根据用户是否在线标志判断，如果不在线
    if(notOnExists)
    {
        //将用户加入用户表
        UserList.append(userTemp);
    }
    //重置用于显示的控件
    ui->listWidget_ALL_user->clear();
    //遍历用户表
    for(int i=0;i<UserList.length();i++)
    {
        //使用控件逐行显示用户表的内容
        ui->listWidget_ALL_user->addItem(QString(UserList.at(i).UserName).append(QString("  ID:%1").arg(UserList.at(i).Id)).append("   IP:").append(UserList.at(i).Ip));
    }
    //TODO:实现接收广播后判断用户是否在线功能
}

//发送广播信号
void Widget::UdpSendBoardCast(QString Name, int Id)
{
    emit sendBroadCast(Name,Id);
}

/*//设置连接地址和端口
*void Widget::SetConnectInfo(QHostAddress ip, int port)
*{
*    emit setIpAndPort(ip,port);
*}
*/

//通过UDP发送数据
void Widget::UdpSendData(QByteArray send)
{
    emit udpSend(send);
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
//    DataFromUdp = data;//存储接收到的内容 TODO:实现回放功能
    emit AudioReceived(data);//通过信号将音频数据传入音频播放线程    
    //遍历用户表
    //?:这个是不是耗时操作？如果是，应该在别的线程执行还是应该在别的函数执行
    for(int i=0;i<UserList.length();i++)
    {
        if(UserList.at(i).Id==SpeakerId)
        {
            if(SpeakingFlag<1)
            {
                //标记正在说话的用户
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
