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

    //设定屏幕位置
    this->setFixedSize(1100, 780);
    this->move(400, 125);

    //初始化游戏
    initgame();
    game->startgame(MAN_TO_MAN);
}

p_pwindow::~p_pwindow()
{
    delete ui;
    delete game;
}

void p_pwindow::initgame(){
    game = new Game;
}

void p_pwindow::chessbyperson()
{
    if(clickPosCol != -1 && clickPosRow != -1 &&
            game->board[clickPosRow][clickPosCol] != 1 &&
            game->board[clickPosRow][clickPosCol] != -1){
        //考虑落子
        if(game->black_white && game->board[clickPosRow][clickPosCol] != 4 &&
                game->board[clickPosRow][clickPosCol] != 5)
            //放黑子记为1
            game->board[clickPosRow][clickPosCol] = 1;
        else
            //放白子记为-1
            game->board[clickPosRow][clickPosCol] = -1;
        game->black_white = !(game->black_white);
    }
    else return;
    checkboard(); //checkboard()出错
    update();
}
//决定输赢、(AI)统计分数、找出禁手点位
void p_pwindow::checkboard()
{
    //遍历每一个空位
    game->four_ban = 0;
    game->five_ban = 0;
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            if(game->board[i][j] != 0) continue;
            //左斜"\"
            QString str = "";
            for(int ii=i-6, jj=j-6; ii<=i+6; ii++,jj++){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    str += "B";
                }
                else if(game->board[ii][jj] == -1){
                    str += "w";
                }
                else if(game->board[ii][jj] == 0){
                    str += " ";
                }
                else if(game->board[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理左斜"\"
            process(str, j, i);
            //竖线"|"
            str = "";
            qDebug()<<"竖线"<<"str:"<<str<<endl;
            for(int ii=i-6; ii<=i+6; ii++){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i){
                    str += "B";
                }
                else if(game->board[ii][j] == -1){
                    str += "w";
                }
                else if(game->board[ii][j] == 0){
                    str += " ";
                }
                else if(game->board[ii][j] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"|"
            qDebug()<<"处理竖线"<<"str:"<<str<<endl;
            process(str, i, j);
            //右斜"/"
            str = "";
            for(int ii=i-6, jj=j+6; ii<=i+6; ii++, jj--){
                if(ii < 0 || ii > GRID){
                    str += "|";
                }
                else if(ii == i&&jj == j){
                    str += "B";
                }
                else if(game->board[ii][jj] == -1){
                    str += "w";
                }
                else if(game->board[ii][jj] == 0){
                    str += " ";
                }
                else if(game->board[ii][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //处理"/"
            process(str, i, j);
            //横线"-"
            for(int jj=j-6; jj<=j+6; jj++){
                if(jj < 0 || jj > GRID){
                    str += "|";
                }
                else if(jj == j){
                    str += "B";
                }
                else if(game->board[i][jj] == -1){
                    str += "w";
                }
                else if(game->board[i][jj] == 0){
                    str += " ";
                }
                else if(game->board[i][jj] == 1){
                    str += "b";
                }
                else continue;
            }
            //横线"-"
            process(str, i, j);
            //到此，所有的连线类型处理完毕
            if(game->four_ban >1){
                game->board[i][j] = 4;
            }
            if(game->five_ban >1){
                game->board[i][j] = 5;
            }
            //禁手信息录入完毕
            game->four_ban = 0; //注意归零
            game->five_ban = 0;
        }
    }
}
//处理每一种连线类型
void p_pwindow::process(QString &str, int col, int row)
//https://blog.csdn.net/kidults/article/details/80075896(QString 用法)
{
    int len = str.length();

    //四四禁手
    if(str.contains(" Bbbb ") || str.contains(" bbbB ") ||
            str.contains(" bBbb ") || str.contains(" bbBb ")){
        game->four_ban++;
    }

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
            clickPosCol<=GRID && game->board[clickPosRow][clickPosCol] != 1 ||
            game->board[clickPosRow][clickPosCol] != -1){
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
            else if(game->black_white == 1 && (game->board[i][j] == 4 || game->board[i][j] == 5)){
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

    //点禁手位是没有用滴
    if(game->board[clickPosRow][clickPosCol] == 4 ||
            game->board[clickPosRow][clickPosCol] == 5){
        return;
    }
    chessbyperson();

}
