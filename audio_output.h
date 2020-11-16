#ifndef AUDIO_OUTPUT_H
#define AUDIO_OUTPUT_H

#include <QObject>
#include <QThread>
#include <QtMultimedia>
#include <QtDebug>

class Audio_output : public QThread
{
    Q_OBJECT
public:
    explicit Audio_output(QObject *parent =0);
private:
    void run();
    QAudioOutput * output;
    QIODevice * outputdevice;


private slots:
    void OnAudiooutputting(char* audio);
//    void Audiooutput();
};

#endif // AUDIO_OUTPUT_H
