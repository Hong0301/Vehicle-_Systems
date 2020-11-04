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

    // ʹ�ð�ť���Ա�����
    ui->pushButtonStarStop->setCheckable(true);
    ui->pushButtonStarStop->setChecked(false);

    // һ��ʼ�����á��ƴΡ���ť
    ui->pushButtonCountReset->setEnabled(false);

    // ���Ӷ�ʱ�����ź����
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
        // ÿ��1���룬����һ��timeout�ۺ���
        // �����1���룬������ʵ�������ŵ�ʱ��
        // ���ǳ���ִ�м����1����
        timer.start(1);

        // ��¼��ǰ����ʵ�����ʱ��
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
        // �����������б�����
        ui->listWidget->addItem(ui->label->text());
    }

    if(ui->pushButtonCountReset->text() == "Reset")
    {
        // �б����㣬��ʱ������
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
    // ��ȡ�Ӽ������Ԫ��㵽���ڵĺ�����
    // �������Ԫ���: 1970��1��1�����
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
