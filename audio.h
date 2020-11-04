#ifndef AUDIO_H
#define AUDIO_H

#include <QWidget>
#include <QProcess>
#include <QString>
#include <QScrollBar>
#include <QIcon>
#include <QTextCodec>
#include <QTimer>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QCoreApplication>
#include <QLabel>
#include <QList>
namespace Ui {
class audio;
}

class audio : public QWidget
{
    Q_OBJECT

public:
    explicit audio(QWidget *parent = nullptr);
    ~audio();
    void showLrc(QString mp3Name);





private slots:
    void on_music_pause_clicked();

    void on_music_start_clicked();

    void on_exitToMain_clicked();


    void  my_func();
    void  sing_func();

    void on_setVolume_valueChanged(int value);

    void back_message_slots();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::audio *ui;
    QLabel *label_1;
    QLabel *label_2;
    QTimer *timer;
    int iCount=0;

    QProcess  *music_play;
    QTimer time;
    QStringList songlist;

};



#endif // AUDIO_H
