#include "audio_output.h"
#include "widget.h"


Audio_output::Audio_output(QObject *parent):QThread(parent)
{
    moveToThread(this);
}

void Audio_output::run()
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);

    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }
    output = new QAudioOutput(format,this);
    outputdevice = output->start();
    this->exec();
}

void Audio_output::OnAudiooutputting(char *audio)
{
    outputdevice->write(audio,640);
    audio=nullptr;
    //写入播放设备的内存
//    qDebug()<<qstrlen(audio);
    //qDebug()<<audio<<"length"<<audio.length()<<"ThreadId"<<this->thread()->currentThreadId();//内容信息
}
