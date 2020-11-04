#include "audio.h"
#include "ui_audio.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPaintEvent>

int music_flag = 0;
int i=0;
audio::audio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::audio)
{
    ui->setupUi(this);
    music_play = new QProcess(this);
    //设置文件路径
    //QString video_dirpath = "C:\\Users\\asus\\Desktop\\qt_pro";
    //QString music_dirpath = "./";
    QString music_dirpath = "./";
    //设置要遍历的目录
    QDir dir(music_dirpath);

    //设置文件过滤器
    QStringList file_name;

    //设置音频文件过滤格式
    file_name << "*.mp3";

    //将过滤后的文件名称存入到all_video_list列表中
    QStringList all_music_list = dir.entryList(file_name, QDir::Files|QDir::Readable, QDir::Name);

    //将all_video_list添加到下拉框
    ui->music_comboBox->addItems(all_music_list);



    /*****************************/
    label_1=new QLabel(this);
    label_1->move(80,120);
    label_2=new QLabel(this);
    label_2->move(80,120);
    label_2->setStyleSheet("color:red");
    label_1->setText(songlist[9]);




    timer=new QTimer(this);//计时器
    //歌词速度
    timer->start(500);//计时周期100ms,并开始计时
    connect(timer,SIGNAL(timeout()),this,SLOT(my_func()));


    connect(music_play,SIGNAL(readyReadStandardOutput()),this,SLOT(back_message_slots()));
    connect(music_play,SIGNAL(readyReadStandardOutput()),this,SLOT(sing_func()));


}



audio::~audio()
{
    delete ui;
}

void audio::back_message_slots()
  {
//      while(music_play->canReadLine())
//     {
//         QString message(music_play->readLine());
//        //message 即为读取的信息我们可以根据需要取我们要的信息如
//        //文件总时间为：ANS_LENGTH=23.00
//        //当前时间为：ANS_TIME_POSITION=23.00
//         qDebug()<<message;
//         ui->current_time->setText(message);
//      }

      music_play->write("get_time_length\n");
      music_play->write("get_time_pos\n");
      music_play->write("get_percent_pos\n");
      while(music_play->canReadLine())
      {
          QByteArray b(music_play->readLine());
          b.replace(QByteArray("\n"), QByteArray(""));
          QString s(b);

          if (b.startsWith("ANS_LENGTH"))  //输出音乐总时间
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
             // ui->video_total_time->setText(totalTime);
              ui->all_time->setText(totalTime);
             // ui->video_slider->setRange(0,totalTimeNum);
              ui->singSlider->setRange(0,totalTimeNum);
          }

          else if (b.startsWith("ANS_TIME_POSITION"))  //输出当前时间
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
              //ui->video_current_time->setText(currentTime);
                ui->current_time->setText(currentTime);
              //ui->video_slider->setValue(currentTimeNum);
                ui->singSlider->setValue(currentTimeNum);
          }

          else if(b.startsWith("ANS_PERCENT_POSITION"))
          {
              QString currentPercent = s.mid(21);
              //ui->video_percentage->setText(currentPercent+"%");//视频播放进度百分比
              ui->abc->setText(currentPercent+"%");
          }

      }


   }

void audio::on_exitToMain_clicked()
{
    if(music_play)
    {
        music_play->kill();
        music_play->close();

        qDebug()<<"音乐进程回收成功";
    }
    else
    {
        qDebug()<<"music defeat!";
    }

    //回收进程资源
    if(music_play->waitForFinished())
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

void audio::my_func()
{


    int iLength = label_1->text().length();
    iCount = (iCount+1)%iLength;
    label_2->setText(label_1->text ().mid(0,iCount+1));
   // ui->listWidget_2->addItem(ui->listWidget_2->item(i++)->text().mid(0,iCount+1));
    label_2->adjustSize();

}

void audio::sing_func()
{

}
/*
void audio::sing_func()
{
    ui->listWidget->addItem(a);
    int index = a.indexOf('\n');
    if(index != -1)
    {//根据播放时间，设置当前显示行
        //ui->listWidget->setCurrentRow(index);
        //设置正在播放的歌词字体变大、颜色为红色
        ui->listWidget->item(index)->setFont(QFont(tr("微软雅黑"),12));
        ui->listWidget->item(index)->setTextColor(Qt::white);
        if(index > 1)
        {//上一行改的字体与颜色还原
            ui->listWidget->item(index-1)->setFont(QFont(tr("微软雅黑"),10));
            ui->listWidget->item(index-1)->setTextColor(Qt::black);
        }
        if(index >5)
        {//播放到第5行时，移动滚动条位置，使用当前显示的歌词居中显示
            ui->listWidget->verticalScrollBar()->setSliderPosition(index-5);
        }
    }
}
*/
void audio::on_music_start_clicked()
{
    if(music_flag == 0)
    {
        qDebug()<<"当前无在播放的音乐！";
    }
    else
    {
        music_play->kill();

        //回收进程资源
        if(music_play->waitForFinished())
        {
            qDebug() << "wait ok";
        }
        else
        {
            qDebug() << "wait fail";
        }
    }

    QString current_choose_music = ui->music_comboBox->currentText();
    QString music_play_command = "mplayer -slave -quiet ./";
    music_play_command += current_choose_music;

    qDebug()<<current_choose_music;
    music_play->start(music_play_command);  
    music_flag++;
}




void audio::on_music_pause_clicked()
{
    QString  music_pause_cmd = "pause\n";  //暂停
    music_play->write(music_pause_cmd.toUtf8());
}



//调正音量
void audio::on_setVolume_valueChanged(int value)
{
    music_play->write(QString("volume %1 2\n").arg(value).toUtf8());
}


/*
//显示歌词
void audio::showLrc(QString mp3Name)
{
    mp3Name.chop(3);
    QString lrcName = mp3Name + "lrc";
    qDebug() << "lrc : " << lrcName;
    //迭代器
    QStringList::const_iterator it;
    it = qFind(lrcList,lrcName);
    if(it == lrcList.end())     //没有找到对应的歌词
    {
        //清空列表
        ui->listWidget->clear();
        return ;
    }
    lrcTimeList.clear();    //清空时间列表
    ui->listWidget->clear();
    ui->listWidget->verticalScrollBar()->setSliderPosition(0);
    QString absLrcName = musicPath + lrcName;
    QFile file(absLrcName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open lrc error" << file.errorString();
        return ;
    }
    QTextStream in(&file);
    in.setCodec("GBK");
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QRegExp reg("\\[[0-9]{2,2}:[0-9]{2,2}\\.[0-9]{2,2}\\]");
        bool ok = line.contains(reg);
        if(ok)
        {
             //[01:20.31]    01  20  3,精确到百毫秒
            double t = line.mid(1,2).toInt()*60 + line.mid(4,2).toInt() + line.mid(7,1).toDouble()/10;
            lrcTimeList << t;
            line.remove(reg);
            QListWidgetItem *item = new QListWidgetItem(line);
            item->setTextAlignment(Qt::AlignCenter);
            ui->listWidget->addItem(item);
        }
    }


}

*/


void audio::on_pushButton_3_clicked()
{
    /*
    QString  music_pause_cmd = "volume -5\n";
    music_play->write(music_pause_cmd.toUtf8());*/
}

void audio::on_pushButton_2_clicked()
{
   /* QString  music_pause_cmd = "volume +5\n";
    music_play->write(music_pause_cmd.toUtf8());*/
}

