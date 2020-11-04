#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    minutes(0),
    seconds(0),
    msec(0)
{
    ui->setupUi(this);

    // 使得按钮可以被按下
    ui->pushButtonStarStop->setCheckable(true);
    ui->pushButtonStarStop->setChecked(false);

    // 一开始，禁用“计次”按钮
    ui->pushButtonCountReset->setEnabled(false);

    // 连接定时器的信号与槽
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonStarStop_clicked(bool checked)
{
    if(checked)
    {
        // 每隔1毫秒，触发一次timeout槽函数
        // 这里的1毫秒，不是真实世界流逝的时间
        // 而是程序执行间隔的1毫秒
        timer.start(1);

        // 记录当前的真实世界的时间
        begin = QDateTime::currentMSecsSinceEpoch();

        ui->pushButtonStarStop->setText("Stop");
        ui->pushButtonCountReset->setText("Cout");
        ui->pushButtonCountReset->setEnabled(true);
    }
    else
    {
        timer.stop();

        ui->pushButtonStarStop->setText("GO");
        ui->pushButtonCountReset->setText("Reset");
    }
}

void Widget::on_pushButtonCountReset_clicked()
{
    if(ui->pushButtonCountReset->text() == "Cout")
    {
        // 计数，增加列表内容
        ui->listWidget->addItem(ui->label->text());
    }

    if(ui->pushButtonCountReset->text() == "Reset")
    {
        // 列表清零，计时器归零
        ui->listWidget->clear();
        ui->label->setText("00:00:000");

        minutes = 0;
        seconds = 0;
        msec=  0;


        ui->pushButtonCountReset->setEnabled(false);
        ui->pushButtonCountReset->setText("Cout");
    }
}

void Widget::onTimeOut()
{
    // 获取从计算机纪元零点到现在的毫秒数
    // 计算机纪元零点: 1970年1月1日零点
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 dt = now - begin;

    msec = dt%1000;
    seconds = (dt/1000) % 60;
    minutes = (dt/1000) / 60;

    ui->label->setText(QString("%1:%2:%3").arg(minutes,2,10,QChar('0'))
                                          .arg(seconds,2,10,QChar('0'))
                                          .arg(msec,3,10,QChar('0')));
}


void Widget::on_pushButton_clicked()
{
    MainWindow *W = new MainWindow;
    W->show();
    this->hide();
}
