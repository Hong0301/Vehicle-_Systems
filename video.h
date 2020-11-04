#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class video;
}

class video : public QWidget
{
    Q_OBJECT

public:
    explicit video(QWidget *parent = nullptr);
    ~video();

private slots:


    void on_start_play_clicked();

    void on_exitToMain_clicked();

    void on_go_ahead_clicked();

    void on_go_back_clicked();

    void on_pause_play_clicked();

    void on_over_play_clicked();

    void dataReceive();

    void on_video_slider_sliderReleased();

    void on_video_slider_valueChanged(int value);

    void on_video_mute_clicked();

private:
    Ui::video *ui;
    QProcess *video_play;
    bool video_flag;

    int totalTimeNum;

    int video_mute_flag = 0;
};

#endif // VIDEO_H
