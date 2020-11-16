#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QThread>
#include <QtNetwork>
#include <QtDebug>
#include <QMetaType>

struct UdpData{
    int Flag;
    char UserName[50];
    int UserId;
    char Adata[1024];
};



class UdpSocket : public QThread
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject*parent = 0);
    int selfId;
private:
    void run();
    QUdpSocket *udpSendSocket;
    QUdpSocket *udpReceiveSocket;
    QHostAddress ReceiveIp;
    int Port;

    QByteArray datagram;
    UdpData *ReceiveData;
    UdpData broadCast;

private slots:
    //void SetBind();
    void ReadData();
    void SendDataUdp(QByteArray sendData);
    void SendBroadCast(QString Name, int Id);
signals:
    void ReceiveBroadCast(QString Name,int Id,QByteArray address,QByteArray Adata);
    void MessageFromUdp(QByteArray Message);
    void AudioFromUdp(char* Message , int SpeakerId);
    void SendBroadCastError(bool Error);
};

#endif // UDPSOCKET_H
