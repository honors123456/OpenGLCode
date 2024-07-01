#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>

#define PI 3.1415926

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    btn=new QPushButton(this);
    connect(btn,&QPushButton::clicked,this,&Widget::slotUpdateWin);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //设置坐标系
    QPointF center(width() / 2.0, height() / 2.0);
    painter.translate(center);

    //通用DDA直线算法
    for(int angle=0;angle<=360;angle+=5)
    {
        double r=300;
        double x=r*cos(angle*PI/180);
        double y=r*sin(angle*PI/180);
        DDALine(&painter,QPoint(0,0),QPoint(qRound(x),qRound(y)));
    }
}

void Widget::slotUpdateWin()
{
    //update();
    repaint();
}

void Widget::DDALine(QPainter *painter, QPoint p1, QPoint p2)
{
    double dx=p2.x()-p1.x();
    double dy=p2.y()-p1.y();

    double max=fmax(abs(dx),abs(dy));

    double stepX=dx/max;
    double stepY=dy/max;

    for(int i=0;i<int(max);i++)
    {
        painter->setPen(Qt::red);
        double x=p1.x();
        double y=p1.y();

        painter->drawPoint(QPoint(qRound(x),qRound(y)));
        //painter->drawLine(0,0,100,100);

        p1.setX(x+=stepX);
        p1.setY(y+=stepY);
    }
}

