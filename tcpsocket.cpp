#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *p):QThread(p)
{
    moveToThread(this);
}

void TcpSocket::run()
{
    socket = new QTcpSocket(this);
    server = new QTcpServer(this);
    TcpSender = new QTcpSocket(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(NewConnection()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(socket_read()));
    this->exec();
}

void TcpSocket::NewConnection()
{
    socket = server->nextPendingConnection();
    qDebug()<<server->nextPendingConnection()->localAddress();
    connect(socket,SIGNAL(readyRead()),SLOT(socket_read()));
}

void TcpSocket::setPortAndIp(QString ip, int port)
{
    server->close();
    server->listen(QHostAddress::Any,port);
    Ip=ip;
    Port=port;
    qDebug()<<ip<<port;
}

void TcpSocket::socket_read()
{
    QByteArray buffer;
    buffer = socket->readAll();
    socket->flush();
    emit MessageFromTcp(buffer);
}

void TcpSocket::sendDataTcp(QByteArray data)
{
    TcpSender->connectToHost(Ip,Port);
    TcpSender->flush();
    if(TcpSender->waitForConnected(3000))
    {
        TcpSender->write(data);
        TcpSender->disconnectFromHost();
    }
    else
    {
        qDebug("failed");
    }
}
