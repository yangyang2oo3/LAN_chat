#include "audio_input.h"

Audio_input::Audio_input(QObject * parent):QThread(parent)
{
    moveToThread(this);
}

//线程入口
void Audio_input::run()
{
    //声明一个音频格式对象
    QAudioFormat format;
    /*设置音频的格式*/
    //设置采样率
    format.setSampleRate(8000);
    //设置声道个数（1平声道）
    format.setChannelCount(1);
    //设置采样位数
    format.setSampleSize(16);
    //设置声音编码格式
    format.setCodec("audio/pcm");
    //设置低位优先
    format.setByteOrder(QAudioFormat::LittleEndian);
    //设置声音流格式
    format.setSampleType(QAudioFormat::SignedInt);
    /*抽象化的设备类*/
    //声明使用的设备
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    //设置设备的音频格式
    //如果不支持
    if(!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
        //设置和设置的格式最接近并且受支持的格式
    }
    /*抽象化的IO类*/
    //实例化音频输入的抽象类，并设置受支持的编码格式
    input = new QAudioInput(format,this);
    //输入设备启动
    inputdevice = input->start();
    //连接输入设备的信号，用于触发处理发送数据的槽函数
    connect(inputdevice,SIGNAL(readyRead()),SLOT(OnAudioInputting()));
    //线程常驻
    this->exec();
}

//发送数据的处理函数
void Audio_input::OnAudioInputting()
{
    //声明一个数据载体
    QByteArray input_Audio;
    //读取输入设备的内容
    inputdevice->read(Ainput.Adata,640);
    //标志位 1:语音传输 2:UDP广播
    Ainput.Flag = 1;
    //设置用户Id
    Ainput.UserId=UserId;
    //将数据写入数据载体
    input_Audio.append(reinterpret_cast<char*>(&Ainput),sizeof(audioinput));
    //通过信号发送到其他线程
    emit AudioInput(input_Audio);
}
