#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

}

MainWindow::~MainWindow()
{
    delete ui;
}
//进入天气
void MainWindow::on_weather_clicked()
{
    weather *we = new weather;
    we->show();
   this->hide();
}

void MainWindow::on_video_clicked()
{
   video *vi = new video;
    vi->show();
   this->hide();
}

void MainWindow::on_audio_clicked()
{
    Music *a = new Music;
    a->show();
    this->hide();
}
void MainWindow::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    //QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = time.toString("hh:mm:ss");
     QString str2 = time.toString("dddd");

    ui->label->setText(str);
    ui->label1->setText(str1);
    ui->label2->setText(str2);
}


void MainWindow::on_pushButton_clicked()
{
    Widget *a = new Widget;
    a->show();
    this->hide();
}

void MainWindow::on_pushButton_2_clicked()
{
    exit(0);
}
