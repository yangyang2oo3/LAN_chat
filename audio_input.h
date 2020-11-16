#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QThread>
#include <QtMultimedia>
#include <QtDebug>


//定义一个结构体，用于创建数据载体
struct audioinput{
    int Flag;
    char UserName[50];
    int UserId;
    char Adata[1024];
};

class Audio_input : public QThread
{
    Q_OBJECT
public:
    explicit Audio_input(QObject *parent =0);
    //创建一个可以在其他线程访问的类型，用于在其他线程设置用户的Id
    int UserId;
    //同上，设置用户名
    QString UserName;
private:
    void run();
    //使用QAudioInput类获取音频输入
    QAudioInput * input;
    //使用QIODevice定义一个抽象化的IO类
    QIODevice * inputdevice;
    //上面定义的结构体
    audioinput Ainput;
private slots:
    //声明一个槽函数，通过输入信号触发
    void OnAudioInputting();
signals:
    //声明一个信号，然后使用这个信号将数据传出这个线程
    void AudioInput(QByteArray audio);

};

#endif // AUDIO_H
