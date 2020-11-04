#include "video.h"
#include "ui_video.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QString>

int video_play_flag  = 0;
int video_pause_flag = 0;
int currentTimeNum;
int video_slider_temp=0;
video::video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::video)
{
    ui->setupUi(this);
    video_play = new QProcess(this);

    //connect(video_play,SIGNAL(readyRead()),this,SLOT(read_all()));
    connect(video_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
    video_play ->setProcessChannelMode(QProcess::MergedChannels);
    //设置文件路径
    QString video_dirpath = "./";

    //设置要遍历的目录
    QDir dir(video_dirpath);

    //设置文件过滤器
    QStringList file_name;

    //设置视屏文件过滤格式
    file_name << "*.avi";

    //将过滤后的文件名称存入到all_video_list；列表中
    QStringList all_video_list = dir.entryList(file_name, QDir::Files|QDir::Readable,QDir::Name);

    //将all_video_list添加到下拉框

    ui->video_comboBox->addItems(all_video_list);
    ui->video_volume_slider->setRange(0, 100);

     //ui->video_slider->setRange(0, 100);

}

video::~video()
{
    delete ui;
}




void video::on_start_play_clicked()
{
    if(video_play_flag == 0)
    {
        qDebug()<<"当前无在播放的视屏！";
    }
    else
    {
        video_play->kill();

        //回收进程资源
        if(video_play->waitForFinished())
        {
            qDebug() << "wait ok";
        }
        else
        {
            qDebug() << "wait fail";
        }
        video_pause_flag = 0;
    }

    QString current_choose_video = ui->video_comboBox->currentText();
    QString play_command = "mplayer -slave -quiet -zoom  -x 600 -y 360 ./";
    play_command += current_choose_video;

    qDebug()<<current_choose_video;

    video_play->start(play_command);

    video_play_flag++;
}








void video::on_exitToMain_clicked()
{
    //发送 结束信号
    video_play->kill();

    //回收进程资源
    if(video_play->waitForFinished())
    {
        qDebug() << "wait ok";
    }
    else
    {
        qDebug() << "wait fail";
    }

    MainWindow *main = new MainWindow();
    main->show();

    this->hide();

}

void video::on_go_ahead_clicked()
{
    //快进命令
    QString forward_cmd = "seek 5 0\n";  //暂停

    video_play->write(forward_cmd.toUtf8());
}

void video::on_go_back_clicked()
{
    //快进命令
    QString back_cmd = "seek -5 0\n";  //暂停

    video_play->write(back_cmd.toUtf8());
    if(video_pause_flag%2 == 0)
    {

    }
    else
    {
        QString  video_pause_cmd = "pause\n";  //暂停

        video_play->write(video_pause_cmd.toUtf8());
    }

}

void video::on_pause_play_clicked()
{
    if(video_pause_flag%2 == 0)
      {
          disconnect(video_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
      }
      else
      {
          connect(video_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
      }

      QString  video_pause_cmd = "pause\n";  //暂停

      video_play->write(video_pause_cmd.toUtf8());

      video_pause_flag++;
}

void video::on_over_play_clicked()
{
    //发送 结束信号
    video_play->kill();

    //回收进程资源
    if(video_play->waitForFinished())
    {
        qDebug() << "wait ok";
    }
    else
    {
        qDebug() << "wait fail";
    }
}
void video::dataReceive()//接收信息
{
    video_play->write("get_time_length\n");
    video_play->write("get_time_pos\n");
    video_play->write("get_percent_pos\n");
    while(video_play->canReadLine())
    {
        QByteArray b(video_play->readLine());
        b.replace(QByteArray("\n"), QByteArray(""));
        QString s(b);

        if (b.startsWith("ANS_LENGTH"))  //输出视频总时间
        {
            int totalTimeNum = s.mid(11).toFloat();
            int sec = 0, min = 0, hour = 0;
            if(totalTimeNum < 60)
            {
                sec = totalTimeNum;
                min = 0;
                hour= 0;
            }
            if(totalTimeNum >= 60 && totalTimeNum <3600)
            {
                sec = totalTimeNum%60;
                min = totalTimeNum/60;
                hour= 0;
            }
            if(totalTimeNum > 3600)
            {
                sec = totalTimeNum%60;
                min = totalTimeNum/60%60;
                hour= totalTimeNum/3600;
            }

            QString totalTime = QString("%1:%2:%3").arg(hour).arg(min).arg(sec);  //标准格式输出时间
            ui->video_total_time->setText(totalTime);

            ui->video_slider->setRange(0,totalTimeNum);
        }

        else if (b.startsWith("ANS_TIME_POSITION"))  //输出视频当前时间
        {
            int currentTimeNum = s.mid(18).toFloat();
            int sec = 0, min = 0, hour = 0;
            if(currentTimeNum < 60)
            {
                sec = currentTimeNum;
                min = 0;
                hour= 0;
            }
            if(currentTimeNum >= 60 && currentTimeNum <3600)
            {
                sec = currentTimeNum%60;
                min = currentTimeNum/60;
                hour= 0;
            }
            if(currentTimeNum > 3600)
            {
                sec = currentTimeNum%60;
                min = currentTimeNum/60%60;
                hour= currentTimeNum/3600;
            }

            QString currentTime = QString("%1:%2:%3").arg(hour).arg(min).arg(sec);

            ui->video_current_time->setText(currentTime);
             qDebug() << "111";
            ui->video_slider->setValue(currentTimeNum);
            qDebug() << "222";
        }

        else if(b.startsWith("ANS_PERCENT_POSITION"))
        {
            QString currentPercent = s.mid(21);
            ui->video_percentage->setText(currentPercent+"%");//视频播放进度百分比
        }
    }
}

void video::on_video_slider_valueChanged(int value)
{
    //将进度条非零差值保存起来
    if(value - currentTimeNum != 0)
        video_slider_temp = value - currentTimeNum;

    //ui->video_slider->setValue(value);
    qDebug() << "456";
}

void video::on_video_slider_sliderReleased()
{
    QString cmd = QString("seek %1 0\n").arg(video_slider_temp);
    //ui->video_slider->setValue(video_slider_temp);
    video_play->write(cmd.toUtf8());


    qDebug() << "123";
    //差值归零
    video_slider_temp = 0;
}



void video::on_video_mute_clicked()
{
    if(video_mute_flag%2 == 0)
       {
           //开启静音
           video_play->write("mute 1\n");
       }
       else
       {
           //关闭静音
           video_play->write("mute 0\n");
       }

       video_mute_flag++;
}
