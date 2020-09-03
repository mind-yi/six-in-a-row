#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"       //包含头文件

#include <QPushButton>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //按钮设置
    button_man_man = new QPushButton("双人模式", this);
    button_man_ai = new QPushButton("人机模式", this);
    button_ai_ai = new QPushButton("机机模式", this);
    //按钮大小
    button_man_man->setGeometry(820, 90, 200, 100);
    button_man_ai->setGeometry(820, 260, 200, 100);
    button_ai_ai->setGeometry(820, 440, 200, 100);

    //按钮功能
    connect(button_man_man, &QPushButton::released, this, &MainWindow::PP_Slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
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

void MainWindow::PP_Slot()
{
    this->hide();
    win_pp.show();
}

void MainWindow::PA_Slot()
{

}

void MainWindow::AA_Slot()
{

}
