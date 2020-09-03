#include "p_pwindow.h"
#include "ui_p_pwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离

p_pwindow::p_pwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::p_pwindow)
{
    ui->setupUi(this);

    //设定屏幕位置
    this->setFixedSize(1100, 780);
    this->move(400, 125);

    //
}

p_pwindow::~p_pwindow()
{
    delete ui;
}

void p_pwindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
        //绘制棋盘
        painter.setRenderHint(QPainter::Antialiasing, true);// 抗锯齿
        painter.setPen(Qt::BlankCursor);

        for(int i=0; i<GRID+1; i++){
            //从左到右，第(i+1)条竖线
            painter.drawLine(QPointF(MARGIN + INTERVEL*i, MARGIN),
                             QPointF(MARGIN + INTERVEL*i, MARGIN + INTERVEL*20));//https://blog.csdn.net/liang19890820/article/details/51154216
        }
        for(int i=0; i<GRID+1; i++){
            //从上到下，第(i+1)条横线
            painter.drawLine(QPointF(MARGIN, MARGIN + INTERVEL*i),
                             QPointF(MARGIN + INTERVEL*20, MARGIN + INTERVEL*i));
        }
}

void p_pwindow::mouseMoveEvent(QMouseEvent *event)
{
    //通过鼠标的hover确定落子的标记
    int x = event->x();
    int y = event->y();

    //棋子边缘不能落子

}
