#ifndef WEATHER_H
#define WEATHER_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>

namespace Ui {
class weather;
}

class weather : public QWidget
{
    Q_OBJECT

public:
    explicit weather(QWidget *parent = nullptr);
    ~weather();
    QString type_result(QString type);
public slots:
    void connect_ok();
    void read_data();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::weather *ui;
    QTcpSocket *myhttp;

};

#endif // WEATHER_H
