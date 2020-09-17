#include "a_awindow.h"
#include "ui_a_awindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

a_awindow::a_awindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::a_awindow)
{
    ui->setupUi(this);

    game = new Game;
}

a_awindow::~a_awindow()
{
    delete ui;
    delete game;
}

void a_awindow::initgameinterface(bool ban)
{
    //重新开始的时候用的上
    banfrommain = ban;
    game->startgame(AI_TO_AI, ban);
}

void a_awindow::endgame(GameState state)
{
    if(state == BLACK_WIN){
        QMessageBox::about(this, "游戏结束","    黑方获胜    ");
    }
    else if(state == WHITEWIN){
        QMessageBox::about(this, "游戏结束","    白方获胜    ");
    }
    else{
        QMessageBox::about(this, "游戏结束","    和棋    ");
    }
}

void a_awindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //画背景
    painter.drawPixmap(rect(), QPixmap("../marble.jpg"));
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

     QBrush brush;
     brush.setStyle(Qt::SolidPattern);
     //画棋子，画禁手
        for(int i=0; i<=GRID; i++){
            for(int j=0; j<=GRID; j++){
                if(game->board[i][j] == 1){
                    brush.setColor(Qt::black);
                    painter.setBrush(brush);
                    painter.drawEllipse(MARGIN+j*INTERVEL-CHESS_SIZE,
                                        MARGIN+i*INTERVEL-CHESS_SIZE,
                                        CHESS_SIZE*2, CHESS_SIZE*2);
                }
                else if(game->board[i][j] == 2){
                    brush.setColor(Qt::white);
                    painter.setBrush(brush);
                    painter.drawEllipse(MARGIN+j*INTERVEL-CHESS_SIZE,
                                        MARGIN+i*INTERVEL-CHESS_SIZE,
                                        CHESS_SIZE*2, CHESS_SIZE*2);
                }
                //画禁手
                else if(game->black_white == 1 && game->board[i][j] == 4 && game->banprompt){

                    brush.setColor(Qt::red);
                    painter.setBrush(brush);
                    painter.drawRect(MARGIN+j*INTERVEL-PIX_SIZE,
                                     MARGIN+i*INTERVEL-PIX_SIZE,
                                     PIX_SIZE*2, PIX_SIZE*2);
                }
                else continue;
            }
        }
}


void a_awindow::on_button_start_clicked()
{
    ui->button_start->setText("继续");
    //如果游戏有胜负了，AI就没有必要再下
    if(game->gamestate != NOWINNER){
        return;
    }
    GameState state;
    state = game->chessbyai();
    update();
    if(state == BLACK_WIN || state == WHITEWIN){
        endgame(state);
    }
}

void a_awindow::on_button_restart_clicked()
{
    QMessageBox::warning(this, "提示", "游戏重新开始");
    game->startgame(AI_TO_AI, banfrommain);
    ui->button_start->setText("开始");
}

void a_awindow::on_button_return_clicked()
{

    emit aasignal();
    //处理玩信号后，仍然会执行
    ui->button_start->setText("开始");
}
