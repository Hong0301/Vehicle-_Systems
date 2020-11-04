#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer> // 定时器
#include <QDateTime> // 获取真实世界的事件
#include <QTime>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButtonStarStop_clicked(bool checked);
    void on_pushButtonCountReset_clicked();
    void onTimeOut();

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QTimer timer;

    qint64 begin;

    int minutes;
    int seconds;
    int msec;
};

#endif // WIDGET_H
