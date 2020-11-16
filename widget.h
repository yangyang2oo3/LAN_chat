#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include "tcpsocket.h"
#include "udpsocket.h"
#include "audio_input.h"
#include "audio_output.h"
#include <QSettings>
#include <QTimer>
#include <QListWidgetItem>

struct User{
    QString UserName;
    int Id;
    QString Ip;
    QByteArray Adata;
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

private:
    Ui::Widget *ui;

    QString username;

//    TcpSocket * tcpSocket;

    UdpSocket * udpSocket;

    Audio_input * audio_input;

    Audio_output * audio_output;

    QByteArray DataFromTcp;

    QByteArray DataFromUdp;

    QByteArray AudioInput;

    QSettings  *UserInfo;

    QVector<User> UserList;

    int SpeakingFlag;

    QTimer *RefuseUserList;

private slots:
    //接收网络通信数据的槽函数

//    void ReceiveTcpData(QByteArray data);
    //获取Tcp接收到的数据
    void ReceiveAudioData(char *data, int SpeakerId);
    //获取Udp接收到的数据
    //音频通讯
    void get_Audio_input(QByteArray Audio);
    //获得音频数据
    void ReceiveBroadCast(QString Name, int Id, QByteArray address, QByteArray Adata);
    //获取他人广播的内容
    void ReSendBoardCast();
private:
    //与网络通信线程交换数据的函数

    void UdpSendData(QByteArray send);
    //获取udp
//    void TcpSendData(QByteArray send);
    //通过tcp发送数据
    void SetConnectInfo(QHostAddress ip, int port);
    //设置连接信息
    void initNetwork();
    //初始化程序的网络设置
    void UdpSendBoardCast(QString Name, int Id);
signals:
    //与网络通信线程交换数据的信号
    void tcpSend(QByteArray sendTcp);
    //通过Tcp发送数据
    void udpSend(QByteArray sendUdp);
    //通过Udp发送数据
    void setIpAndPort(QHostAddress ip,int port);
    //设置端口号和ip地址
    void AudioReceived(char* audio);
    //播放接收到的数据
    void sendBroadCast(QString Name,int Id);
    //发送广播自己身份的信息
private slots:
    void on_pushButton_Enter_clicked();
    void on_pushButton_Exit_clicked();
    void on_checkBox_Mic_clicked(bool checked);
};
#endif // WIDGET_H
