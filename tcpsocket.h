#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QThread>
#include <QtNetwork>
#include <QtDebug>

//创建一个线程来获取客户端传来的数据

class TcpSocket : public QThread
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject * p = 0);

private:
    void run();
    QTcpServer * server;
    QTcpSocket * TcpSender;
    QTcpSocket * socket;
    int Port;
    QString Ip;
private slots:
    void socket_read();
    void NewConnection();
    //发送数据
    void sendDataTcp(QByteArray data);
    //设置IP地址和端口
    void setPortAndIp(QString ip,int port);
    //获取接收到的数据

signals:
    void MessageFromTcp(QByteArray Message);
};

#endif // TCPSERVER_H
