#include "udpsocket.h"

UdpSocket::UdpSocket(QObject*parent):
      QThread(parent)
{
    moveToThread(this);
}

void UdpSocket::run()
{
    udpSendSocket = new QUdpSocket(this);
    udpReceiveSocket = new QUdpSocket(this);
    ReceiveData=nullptr;
    connect(udpReceiveSocket,SIGNAL(readyRead()),SLOT(ReadData()));
    udpReceiveSocket->bind(QHostAddress::Any,30108);
    this->exec();
}

void UdpSocket::SendDataUdp(QByteArray sendData)
{
    udpSendSocket->writeDatagram(sendData,QHostAddress::Broadcast,30108);
//    qDebug()<<sendData.length();
//    qDebug()<<Port;
}

//void UdpSocket::SetBind()
//{

//}

void UdpSocket::ReadData()
{
    ReceiveData=nullptr;
    if(udpReceiveSocket->hasPendingDatagrams())
    {

        datagram.resize(udpReceiveSocket->pendingDatagramSize());
        udpReceiveSocket->readDatagram(datagram.data(),datagram.size(),&ReceiveIp);

        ReceiveData=(UdpData*)(datagram.data());
        //qDebug()<<"ReceiveData = "<<ReceiveData<<",datagram.data()"<<datagram.data();
//        qDebug()<<ReceiveIp;
    }
    /*Flag:1 音频信息;
     *Flag:2 广播用户信息;
     *Flag:3 待定 ;*/
    ReceiveIp.setAddress(ReceiveIp.toIPv4Address());
//    qDebug()<<ReceiveIp.toString();
//    qDebug()<<selfId;
    switch (ReceiveData->Flag)
    {
    case 1:
        if(ReceiveData->UserId!=selfId){
            emit AudioFromUdp(ReceiveData->Adata,ReceiveData->UserId);
        }
        break;
    case 2:
        emit ReceiveBroadCast(ReceiveData->UserName,ReceiveData->UserId,ReceiveIp.toString().toLocal8Bit(),ReceiveData->Adata);
        //qDebug()<<"UdpSocket ReceiveId:"<<ReceiveData->UserId;
        break;
    case 3:
        break;
    }
//    qDebug()<<ReceiveData->Adata;
}

void UdpSocket::SendBroadCast(QString Name,int Id)
{
    bool Error= false;

    QByteArray sendData;
    broadCast.Flag=2;
    broadCast.UserId=Id;
    if(Name.length()<=50&&Name!="")
    {
        strcpy(broadCast.UserName,Name.toLocal8Bit());

    }else{
        Error=true;
    }
    if(Error)
    {
        emit SendBroadCastError(Error);
    }
    else{
        sendData.append(reinterpret_cast<char*>(&broadCast),sizeof (UdpData));
        udpSendSocket->writeDatagram(sendData,QHostAddress::Broadcast,30108);
    }

}


