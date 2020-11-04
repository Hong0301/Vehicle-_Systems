#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "weather.h"
#include "ui_weather.h"
#include <QWidget>
#include "video.h"
#include "ui_video.h"
#include "audio.h"
#include "ui_audio.h"
#include <QTimer>
#include "music.h"
#include "ui_music.h"
#include "widget.h"
#include "ui_widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void timerUpdate(void);


private slots:
    void on_weather_clicked();

    void on_video_clicked();

    void on_audio_clicked();



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
