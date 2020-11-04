#ifndef MUSIC_H
#define MUSIC_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class Music;
}

class Music : public QWidget
{
    Q_OBJECT

public:
    explicit Music(QWidget *parent = nullptr);
    ~Music();

private slots:
    void on_exitToMain_clicked();
    void on_music_start_clicked();
    void on_music_pause_clicked();
    void on_volume_up_clicked();
    void on_volume_down_clicked();
    void dataReceive();

    void on_music_volume_slider_valueChanged(int value);

    void on_music_slider_valueChanged(int value);

    void on_music_slider_sliderReleased();

    void on_music_next_clicked();

    void on_music_prev_clicked();

    void on_go_head_clicked();

    void on_go_back_clicked();

private:
    Ui::Music *music_ui;
    QProcess  *music_play;

    int music_flag = 0;
    int music_pause_flag = 0;

    int totalTimeNum;
    int currentTimeNum;

    int music_slider_temp;

    QStringList all_music_list;
    QString current_choose_music;
};

#endif // MUSIC_H
