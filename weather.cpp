#include "weather.h"
#include "ui_weather.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonObject>

weather::weather(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weather)
{
    ui->setupUi(this);

}

weather::~weather()
{
    delete ui;
}
//提示是否连接成功,不成功不提示
void weather::connect_ok()
{
    QMessageBox::warning(this,tr("连接提醒"),tr("连接成功"),QMessageBox::Ok);
}

//判断天气
QString weather::type_result(QString type)
{
    QString path;
    if(type == "晴"){path = ":/myimage/pic/1.png";}
    else if(type=="多云" || type == "晴转多云"){path = ":/myimage/pic/2.png";}
    else if(type=="阴"){path = ":/myimage/pic/3.png";}
    else if(type=="阵雨"){path = ":/myimage/pic/4.png";}
    else if(type=="雷阵雨"){path = ":/myimage/pic/4.png";}
    else if(type=="小雨"){path = ":/myimage/pic/5.png";}
    else if(type=="中雨"){path = ":/myimage/pic/7.png";}
    else if(type=="大雨"){path = ":/myimage/pic/7.png";}
    else if(type=="暴雨"){path = ":/myimage/pic/7.png";}
    else if(type =="大暴雨"||type =="大到暴雨"){path = ":/myimage/pic/7.png";}
    else {path = ":/myimage/pic/1.png";}
    return path;
}

//读取返回的数据
void weather::read_data()
{
    QString msg = myhttp->readAll();

    //处理字符串
    msg = msg.remove(0,msg.indexOf("\r\n\r\n")+9);
    msg.chop(6);
    //ui->textBrowser->setText(msg);
    QByteArray bytes = msg.toUtf8();
    QJsonParseError er;
    QJsonDocument myJson_msg = QJsonDocument::fromJson(bytes,&er);

    if(er.error == QJsonParseError::NoError){
         qDebug()<<"当前 JSON数据初始化成功";
    }
    else{
           qDebug()<<"JSON数据初始化失败";
    }

    QJsonObject first_obj = myJson_msg.object();    //大对象

    //通过KEY 获取 value
    QJsonValue result_value = first_obj.take("result");  //大对象的value:字符串
    QJsonValue content_value  = first_obj.take("content");   //大对象的value:字符串
    QString allstring = content_value.toString();
    //ui->textBrowser->setText(allstring);

    //分割出当前天气
    QString allstring1=allstring;
    QString string =allstring1.remove(allstring1.indexOf("{br}"),allstring1.size());
    //ui->textBrowser->append(string);
    QString place=string;
    place=place.remove(2,string.size());
    ui->label_space->setText(place);   //显示地点，广州
    QString tempu=string;
    tempu=tempu.mid(10,3);
    ui->label_temp->setText(tempu);    //你7
    QString sentence=string;
    sentence=sentence.mid(14);
    ui->label_sentence->setText(sentence);

    //今天的天气
    QString allstring2=allstring;
    QString string1 =allstring2.remove(0,allstring2.indexOf("{br}")+4);
    QString string2 = string1;
    string1 = string1.remove(string1.indexOf("{br}"),string1.size());
    //ui->textBrowser->append(string1);
    QString day11 = string1;

    ui->label_day1_1->setText(day11.mid(1,6));                  //日期
    day11 =day11.remove(day11.indexOf("低温")-1,day11.size());
    day11 =day11.remove(0,day11.indexOf("：")+1);
    ui->label_day1_2->setText(day11);                           //天气

    QString path1=type_result(day11);                            //图标
    ui->label_pic1->setScaledContents(true);                  //自动适应大小
    ui->label_pic1->setPixmap(QPixmap(path1));

    QString day12 = string1;
    day12 =day12.remove(0,day12.indexOf("，")+1);
    QString day13 = day12;
    day12 =day12.remove((day12.indexOf("，")),day12.size());
    ui->label_day1_3->setText(day12);                          //低温

    day13=day13.remove(0,day13.indexOf("，")+1);
    QString day14 = day13;
    day13 =day13.remove((day13.indexOf("，")),day13.size());
    ui->label_day1_4->setText(day13);                         //高温

    day14=day14.remove(0,day14.indexOf("，")+1);
    ui->label_day1_5->setText(day14);                          //风力



    //明天的天气
    string2=string2.remove(0,allstring2.indexOf("{br}")+4);
    QString string3 = string2;
    string2 = string2.remove(string2.indexOf("{br}"),string2.size());
    //ui->textBrowser->append(string2);

    QString day21 = string2;

    ui->label_day2_1->setText(day21.mid(1,6));                  //日期
    day21 =day21.remove(day21.indexOf("低温")-1,day21.size());
    day21 =day21.remove(0,day21.indexOf("：")+1);
    ui->label_day2_2->setText(day21);                           //天气

    QString path2=type_result(day21);                            //图标
    ui->label_pic2->setScaledContents(true);                  //自动适应大小
    ui->label_pic2->setPixmap(QPixmap(path2));

    QString day22 = string2;
    day22 =day22.remove(0,day22.indexOf("，")+1);
    QString day23 = day22;
    day22 =day22.remove((day22.indexOf("，")),day22.size());
    ui->label_day2_3->setText(day22);                          //低温

    day23=day23.remove(0,day23.indexOf("，")+1);
    QString day24 = day23;
    day23 =day23.remove((day23.indexOf("，")),day23.size());
    ui->label_day2_4->setText(day23);                         //高温

    day24=day24.remove(0,day24.indexOf("，")+1);
    ui->label_day2_5->setText(day24);                          //风力

    //后天的天气
    string3=string3.remove(0,string3.indexOf("{br}")+4);
    QString string4 = string3;
    string3 = string3.remove(string3.indexOf("{br}"),string3.size());
    //ui->textBrowser->append(string3);

    QString day31 = string3;

    ui->label_day3_1->setText(day31.mid(1,6));                  //日期
    day31 =day31.remove(day31.indexOf("低温")-1,day31.size());
    day31 =day31.remove(0,day31.indexOf("：")+1);
    ui->label_day3_2->setText(day31);                           //天气

    QString path3=type_result(day31);                            //图标
    ui->label_pic3->setScaledContents(true);                  //自动适应大小
    ui->label_pic3->setPixmap(QPixmap(path3));

    QString day32 = string3;
    day32 =day32.remove(0,day32.indexOf("，")+1);
    QString day33 = day32;
    day32 =day32.remove((day32.indexOf("，")),day32.size());
    ui->label_day3_3->setText(day32);                          //低温

    day33=day33.remove(0,day33.indexOf("，")+1);
    QString day34 = day33;
    day33 =day33.remove((day33.indexOf("，")),day33.size());
    ui->label_day3_4->setText(day33);                         //高温

    day34=day34.remove(0,day34.indexOf("，")+1);
    ui->label_day3_5->setText(day34);                          //风力

    //+1天
    string4=string4.remove(0,string4.indexOf("{br}")+4);
    QString string5 = string4;
    string4 = string4.remove(string4.indexOf("{br}"),string4.size());
    //ui->textBrowser->append(string4);

    QString day41 = string4;

    ui->label_day4_1->setText(day41.mid(1,6));                  //日期
    day41 =day41.remove(day41.indexOf("低温")-1,day41.size());
    day41 =day41.remove(0,day41.indexOf("：")+1);
    ui->label_day4_2->setText(day41);                           //天气

    QString path4=type_result(day41);                            //图标
    ui->label_pic4->setScaledContents(true);                  //自动适应大小
    ui->label_pic4->setPixmap(QPixmap(path4));

    QString day42 = string4;
    day42 =day42.remove(0,day42.indexOf("，")+1);
    QString day43 = day42;
    day42 =day42.remove((day42.indexOf("，")),day42.size());
    ui->label_day4_3->setText(day42);                          //低温

    day43=day43.remove(0,day43.indexOf("，")+1);
    QString day44 = day43;
    day43 =day43.remove((day43.indexOf("，")),day43.size());
    ui->label_day4_4->setText(day43);                         //高温

    day44=day44.remove(0,day44.indexOf("，")+1);
    ui->label_day4_5->setText(day44);                          //风力

    //+2天
    string5=string5.remove(0,string5.indexOf("{br}")+4);
    //QString string6 = string5;
    //string5 = string5.remove(string5.indexOf("{br}"),string5.size());
    //ui->textBrowser->append(string5);

    QString day51 = string5;

    ui->label_day5_1->setText(day51.mid(1,6));                  //日期
    day51 =day51.remove(day51.indexOf("低温")-1,day51.size());
    day51 =day51.remove(0,day51.indexOf("：")+1);
    ui->label_day5_2->setText(day51);                           //天气

    QString path5=type_result(day51);                            //图标
    ui->label_pic5->setScaledContents(true);                  //自动适应大小
    ui->label_pic5->setPixmap(QPixmap(path5));

    QString day52 = string5;
    day52 =day52.remove(0,day52.indexOf("，")+1);
    QString day53 = day52;
    day52 =day52.remove((day52.indexOf("，")),day52.size());
    ui->label_day5_3->setText(day52);                          //低温

    day53=day53.remove(0,day53.indexOf("，")+1);
    QString day54 = day53;
    day53 =day53.remove((day53.indexOf("，")),day53.size());
    ui->label_day5_4->setText(day53);                         //高温

    day54=day54.remove(0,day54.indexOf("，")+1);
    ui->label_day5_5->setText(day54);                          //风力



    myhttp->close();
    delete  myhttp;
}
//返回主菜单
void weather::on_pushButton_clicked()
{

    myhttp = new QTcpSocket(this);

    connect(myhttp,SIGNAL(connected()),this,SLOT(connect_ok()));

    myhttp->connectToHost(QHostAddress("47.107.155.132"),80);

    connect(myhttp,SIGNAL(readyRead()),this,SLOT(read_data()));




    QString cityname = ui->comboBox->currentText();
    QString str = QString("GET /api.php?key=free&appid=0&msg=天气%1 HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n").arg(cityname);
//    QString msg = "GET /api.php?key=free&appid=0&msg=你好 HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n";
    if(!cityname.isEmpty())
        myhttp->write(str.toUtf8());
}



void weather::on_pushButton_2_clicked()
{
    MainWindow *W = new MainWindow;
    W->show();
    this->hide();
}

void weather::on_pushButton_3_clicked()
{
    QString cityname = ui->lineEdit->text();
    ui->comboBox->addItem(cityname);
    QMessageBox::warning(this,tr("添加提醒"),tr("添加成功"),QMessageBox::Ok);
    ui->lineEdit->clear();
}

void weather::on_pushButton_4_clicked()
{
     ui->comboBox->currentText().toUpper();
     ui->comboBox->removeItem(0);
     QMessageBox::warning(this,tr("删除提醒"),tr("删除成功"),QMessageBox::Ok);
}
