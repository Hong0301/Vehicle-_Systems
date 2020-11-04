#include "music.h"
#include "ui_music.h"
#include "mainwindow.h"

#include <QDir>
#include <QDebug>
#include <QString>

QMap<QString,QString> display;

Music::Music(QWidget *parent) :
    QWidget(parent),
    music_ui(new Ui::Music)
{
    music_ui->setupUi(this);

    music_play = new QProcess(this);

    //connect(music_play,SIGNAL(readyRead()),this,SLOT(read_all()));
    connect(music_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
    music_play->setProcessChannelMode(QProcess::MergedChannels);

    //�����ļ�·��
    //QString video_dirpath = "C:\\Users\\asus\\Desktop\\qt_pro";
    QString music_dirpath = "./111/";

    //����Ҫ������Ŀ¼
    QDir dir(music_dirpath);

    //�����ļ�������
    QStringList file_name;

    //������Ƶ�ļ����˸�ʽ
    file_name << "*.mp3";

    //�����˺���ļ����ƴ��뵽all_video_list�б���
    all_music_list = dir.entryList(file_name, QDir::Files|QDir::Readable, QDir::Name);

    //��all_video_list��ӵ�������
    music_ui->music_comboBox->addItems(all_music_list);

    //����������
    //music_ui->music_volume_slider->setMinimum(0);
    //music_ui->music_volume_slider->setMaximum(100);
    music_ui->music_volume_slider->setRange(0, 100);

    QPalette  palette = music_ui->music_lyrics->palette();
    palette.setBrush(QPalette::Base, QBrush(Qt::NoBrush));
    music_ui->music_lyrics->setPalette(palette);
}

Music::~Music()
{
    delete music_ui;
}

void Music::on_exitToMain_clicked()
{
    if(music_play)
    {
        music_play->kill();
        music_play->close();

        qDebug()<<"���ֽ��̻��ճɹ�";
    }
    else
    {
        qDebug()<<"music defeat!";
    }

    //���ս�����Դ
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

void Music::on_music_start_clicked()
{
    if(music_flag == 0)
    {
        qDebug()<<"��ǰ���ڲ��ŵ�������";
    }
    else
    {
        music_play->kill();

        //���ս�����Դ
        if(music_play->waitForFinished())
        {
            qDebug() << "wait ok";
        }
        else
        {
            qDebug() << "wait fail";
        }
    }

    current_choose_music = music_ui->music_comboBox->currentText();
    QString music_play_path = "mplayer -slave -quiet ./111/";
    QString music_play_command = "mplayer -slave -quiet ./111/";
    music_play_command += current_choose_music;

    qDebug()<<current_choose_music;

    QString lyrics_file = current_choose_music;
    QStringList lyrics_file_name = lyrics_file.split(".");
    QString lyrics_file_final = "./111/" + lyrics_file_name.at(0) + ".lrc";

    //qDebug()<< lyrics_file_final;


    music_play->start(music_play_command);

    music_flag++;

    display.clear();

        QFile file(lyrics_file_final);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
            qDebug()<<"file path:" + lyrics_file_final;
        }
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            qDebug()<< str;
            QStringList list = str.split("]");
            QString a = list[0];
            QString b = list[1];
                 list = a.split(".");
                    a = list[0];
                    a = a.mid(1);
            display[a]=b;
        }

        music_ui->music_lyrics->clear();
        QMap<QString, QString>::iterator iter = display.begin();
        while (iter != display.end())
        {
            music_ui->music_lyrics->addItem(iter.value());
            iter++;
        }
}

void Music::on_music_pause_clicked()
{
    if(music_pause_flag%2 == 0)
    {
        disconnect(music_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
    }
    else
    {
        connect(music_play,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
    }

    QString  music_pause_cmd = "pause\n";  //��ͣ

    music_play->write(music_pause_cmd.toUtf8());
}

void Music::on_volume_up_clicked()
{
    //������
    QString volume_up = "volume +5\n";

    music_play->write(volume_up.toUtf8());
}

void Music::on_volume_down_clicked()
{
    //������
    QString volume_up = "volume -5\n";

    music_play->write(volume_up.toUtf8());
}

void Music::dataReceive()//������Ϣ
{
    //д���ȡ����Ƶ��ʱ���ź�
    music_play->write("get_time_length\n");

    //д���ȡ��ǰ��Ƶλ���ź�
    music_play->write("get_time_pos\n");

    //д���ȡ��ǰ��Ƶ�ٷֱ��ź��ź�
    music_play->write("get_percent_pos\n");

    while(music_play->canReadLine())
    {
        QByteArray b(music_play->readLine());
        b.replace(QByteArray("\n"), QByteArray(""));
        QString s(b);

        if (b.startsWith("ANS_LENGTH"))  //�����Ƶ��ʱ��
        {
            totalTimeNum = s.mid(11).toFloat();
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

            QString totalTime = QString("%1:%2:%3").arg(hour).arg(min).arg(sec);  //��׼��ʽ���ʱ��
            music_ui->music_total_time->setText(totalTime);

            music_ui->music_slider->setRange(0,totalTimeNum);
        }

        else if (b.startsWith("ANS_TIME_POSITION"))  //�����Ƶ��ǰʱ��
        {
            currentTimeNum = s.mid(18).toFloat();
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

            QString current;
            if(min<=9)
            {
                if(sec<=9)
                    current = QString("0%1:0%2").arg(min).arg(sec);
                else
                    current = QString("0%1:%2").arg(min).arg(sec);
            }
            else
            {
                if(sec<=9)
                    current = QString("%1:0%2").arg(min).arg(sec);
                else
                    current = QString("%1:%2").arg(min).arg(sec);
            }

            music_ui->music_current_time->setText(currentTime);

            music_ui->music_slider->setValue(currentTimeNum);

            QMap<QString, QString>::iterator iter = display.begin();
            while (iter != display.end())
            {
                if(iter.key() == current)
                {
                    for(int i=0;i<music_ui->music_lyrics->count();i++)
                    {
                        if(music_ui->music_lyrics->item(i)->text()==iter.value())
                            music_ui->music_lyrics->setCurrentRow(i);
                    }
                }
                iter++;
            }
        }

        else if(b.startsWith("ANS_PERCENT_POSITION"))
        {
            QString currentPercent = s.mid(21);
            //music_ui->music_percentage->setText(currentPercent+"%");//��Ƶ���Ž��Ȱٷֱ�
        }
    }
}


void Music::on_music_volume_slider_valueChanged(int value)
{
    //������
    QString  video_volume_cmd = QString("volume %1 1\n").arg(value);

    //����
    music_play->write(video_volume_cmd.toUtf8());
    qDebug() << QString("sound %1 %").arg(value);
}

void Music::on_music_slider_valueChanged(int value)
{
    //�������������ֵ��������
    if(value - currentTimeNum != 0)
        music_slider_temp = value - currentTimeNum;

    music_ui->music_slider->setValue(value);
}

void Music::on_music_slider_sliderReleased()
{
    QString cmd = QString("seek %1 0\n").arg(music_slider_temp);

    music_play->write(cmd.toUtf8());

    //��ֵ����
    music_slider_temp = 0;
}

void Music::on_music_next_clicked()
{
    QList<QString>::Iterator it = all_music_list.begin(), itend = all_music_list.end();
    int i = 0;
    for (; it != itend; it++,i++)
    {
        //�ҵ���ǰ������
        if (*it == current_choose_music)
        {
            //����һ�׻�ص���һ�׸�
            if(it == itend)
            {
                it -= (all_music_list.size() - 1) ;
            }
            else
            {
                it++;
            }

            break;
        }
    }

    if(music_flag == 0)
    {
    qDebug()<<"��ǰ���ڲ��ŵ�������";
    }
    else
    {
    music_play->kill();

    //���ս�����Դ
    if(music_play->waitForFinished())
    {
    qDebug() << "wait ok";
    }
    else
    {
    qDebug() << "wait fail";
    }
    }

       current_choose_music = *it;
       QString music_play_path = "mplayer -slave -quiet ./111/";
       QString music_play_command = "mplayer -slave -quiet ./111/";
       music_play_command += current_choose_music;

       qDebug()<<current_choose_music;

       QString lyrics_file = current_choose_music;
       QStringList lyrics_file_name = lyrics_file.split(".");
       QString lyrics_file_final = "./111/" + lyrics_file_name.at(0) + ".lrc";

       //qDebug()<< lyrics_file_final;


       music_play->start(music_play_command);

       music_flag++;

       display.clear();

           QFile file(lyrics_file_final);
           if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
           {
               qDebug()<<"Can't open the file!"<<endl;
               qDebug()<<"file path:" + lyrics_file_final;
           }
           while(!file.atEnd())
           {
               QByteArray line = file.readLine();
               QString str(line);
               qDebug()<< str;
               QStringList list = str.split("]");
               QString a = list[0];
               QString b = list[1];
                    list = a.split(".");
                       a = list[0];
                       a = a.mid(1);
               display[a]=b;
           }

           music_ui->music_lyrics->clear();
           QMap<QString, QString>::iterator iter = display.begin();
           while (iter != display.end())
           {
               music_ui->music_lyrics->addItem(iter.value());
               iter++;
           }
}

void Music::on_music_prev_clicked()
{
    QList<QString>::Iterator it = all_music_list.begin(), itend = all_music_list.end();
    int i = 0;
    for (; it != itend; it++,i++)
    {
        //�ҵ���ǰ������
        if (*it == current_choose_music)
        {
            //����һ�׻�ص����һ�׸�
            if(it == all_music_list.begin())
            {
                it += (all_music_list.size() - 1);
            }
            else
            {
                it--;
            }

            break;
        }
    }

    if(music_flag == 0)
    {
    qDebug()<<"��ǰ���ڲ��ŵ�������";
    }
    else
    {
    music_play->kill();

    //���ս�����Դ
    if(music_play->waitForFinished())
    {
    qDebug() << "wait ok";
    }
    else
    {
    qDebug() << "wait fail";
    }
    }

       current_choose_music = *it;
       QString music_play_path = "mplayer -slave -quiet ./111/";
       QString music_play_command = "mplayer -slave -quiet ./111/";
       music_play_command += current_choose_music;

       qDebug()<<current_choose_music;

       QString lyrics_file = current_choose_music;
       QStringList lyrics_file_name = lyrics_file.split(".");
       QString lyrics_file_final = "./111/" + lyrics_file_name.at(0) + ".lrc";

       //qDebug()<< lyrics_file_final;


       music_play->start(music_play_command);

       music_flag++;

       display.clear();

           QFile file(lyrics_file_final);
           if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
           {
               qDebug()<<"Can't open the file!"<<endl;
               qDebug()<<"file path:" + lyrics_file_final;
           }
           while(!file.atEnd())
           {
               QByteArray line = file.readLine();
               QString str(line);
               qDebug()<< str;
               QStringList list = str.split("]");
               QString a = list[0];
               QString b = list[1];
                    list = a.split(".");
                       a = list[0];
                       a = a.mid(1);
               display[a]=b;
           }

           music_ui->music_lyrics->clear();
           QMap<QString, QString>::iterator iter = display.begin();
           while (iter != display.end())
           {
               music_ui->music_lyrics->addItem(iter.value());
               iter++;
           }
}

void Music::on_go_head_clicked()
{
    //�������
    QString forward_cmd = "seek 5 0\n";  //��ͣ

    music_play->write(forward_cmd.toUtf8());
}

void Music::on_go_back_clicked()
{
    //��������
    QString back_cmd = "seek -5 0\n";  //��ͣ

    music_play->write(back_cmd.toUtf8());
    if(music_pause_flag%2 == 0)
    {

    }
    else
    {
        QString  video_pause_cmd = "pause\n";  //��ͣ

        music_play->write(video_pause_cmd.toUtf8());
    }
}
