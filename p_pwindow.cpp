#include "p_pwindow.h"
#include "ui_p_pwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离
#include <QDebug>

p_pwindow::p_pwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::p_pwindow)
{
    ui->setupUi(this);

    game = new Game;

    //设定屏幕位置
    this->setFixedSize(1100, 780);
    this->move(400, 125);
    //设置询问窗口

}

p_pwindow::~p_pwindow()
{
    delete ui;
    delete game;
}
//初始化对应模式的棋盘
void p_pwindow::initgame(GameType type){
    game->gametype = type;
    //下面布置界面
    if(game->gametype == MAN_TO_MAN){       //双人模式

    }
}

//
//界面与game交会函数（该函数即要用到game里面的数据也要用到界面的数据）
void p_pwindow::chessbyperson()
{
    if(clickPosCol != -1 && clickPosRow != -1 &&
            game->board[clickPosRow][clickPosCol] == 0){
        //考虑落子
        if(game->black_white && game->boardban[clickPosRow][clickPosCol] == false){
            //放黑子记为1
            game->board[clickPosRow][clickPosCol] = 1;
        }
        else
            //放白子记为-1
            game->board[clickPosRow][clickPosCol] = -1;
        //判断输赢
        game->gamestate = game->win_lose(clickPosRow, clickPosCol);
        if(game->gamestate != NOWINNER && game->gamestate != STALEMATE){//胜负已分
            emit finalsignal();
        }
        game->black_white = !(game->black_white);
    }
    else return;
    game->checkboard(); //checkboard()出错
    update();
}

//可能需要多画一个框
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

    //画tracking点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>=0 && clickPosRow<=GRID && clickPosCol>=0 &&
            clickPosCol<=GRID && game->boardban[clickPosRow][clickPosCol]==0){
        if(game->black_white)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(MARGIN+clickPosCol*INTERVEL-CHESS_MATE,
                            MARGIN+clickPosRow*INTERVEL-CHESS_MATE,
                            CHESS_MATE*2, CHESS_MATE*2);
    }

    //画棋子、禁手
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            if(game->board[i][j] == 1){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+j*INTERVEL-CHESS_SIZE,
                                    MARGIN+i*INTERVEL-CHESS_SIZE,
                                    CHESS_SIZE*2, CHESS_SIZE*2);
            }
            else if(game->board[i][j] == -1){
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+j*INTERVEL-CHESS_SIZE,
                                    MARGIN+i*INTERVEL-CHESS_SIZE,
                                    CHESS_SIZE*2, CHESS_SIZE*2);
            }
            //画禁手
            else if(game->black_white == 1 && game->boardban[i][j] == true){

                brush.setColor(Qt::red);
                painter.setBrush(brush);
                painter.drawRect(MARGIN+j*INTERVEL-PIX_SIZE,
                                 MARGIN+i*INTERVEL-PIX_SIZE,
                                 PIX_SIZE*2, PIX_SIZE*2);
            }
            else continue;
        }
    }
    update();
}

//可能要对game进行修改
void p_pwindow::mouseMoveEvent(QMouseEvent *event)
{
    //鼠标位置(x, y)
    int x = event->x();
    int y = event->y();
    //初始化
    clickPosCol = -1;
    clickPosRow = -1;
    //棋子边缘不能落子
    if(x >= MARGIN && x<= MARGIN + INTERVEL*GRID
            && y >= MARGIN && y<= MARGIN + INTERVEL*GRID){
        //获取最近左上角点
        int col = (x - MARGIN)/INTERVEL;
        int row = (y - MARGIN)/INTERVEL;
        //计算左上角点的坐标
        int LeftTopX = MARGIN + col*INTERVEL;
        int LeftTopY = MARGIN + row*INTERVEL;

        double distance;
        //左上角
        distance = sqrt((x-LeftTopX)*(x-LeftTopX)+(y-LeftTopY)*(y-LeftTopY));
        if(distance <= INDEX){
            clickPosCol = col;
            clickPosRow = row;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != -1){
                SelectPos = true;
            }
        }
        //右上角
        distance = sqrt((x-LeftTopX-INTERVEL)*(x-LeftTopX-INTERVEL)+(y-LeftTopY)*(y-LeftTopY));
        if(distance <= INDEX){
            clickPosCol = col + 1;
            clickPosRow = row;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != -1){
                SelectPos = true;
            }
        }
        //左下角
        distance = sqrt((x-LeftTopX)*(x-LeftTopX)+(y-LeftTopY-INTERVEL)*(y-LeftTopY-INTERVEL));
        if(distance <= INDEX){
            clickPosCol = col;
            clickPosRow = row + 1;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != -1){
                SelectPos = true;
            }
        }
        //右下角
        distance = sqrt((x-LeftTopX-INTERVEL)*(x-LeftTopX-INTERVEL)+(y-LeftTopY-INTERVEL)*(y-LeftTopY-INTERVEL));
        if(distance <= INDEX){
            clickPosCol = col + 1;
            clickPosRow = row + 1;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != -1){
                SelectPos = true;
            }
        }
        //完成，目标点的坐标已经存放在clickPosCol和clickPosRow中了
    }
    //之后要重绘
    update();
}

//落完子后就要执行checkboard
void p_pwindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(SelectPos == false)
        return;
    else SelectPos == false;

    //黑子点禁手位是没有用滴,白子可以
    if(game->black_white == 1 && game->board[clickPosRow][clickPosCol] == true){
        return;
    }
    chessbyperson();

}

//结束
void p_pwindow::finalsignal(){
    qDebug()<<"结束了"<<endl;
}
