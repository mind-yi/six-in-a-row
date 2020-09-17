#include "p_pwindow.h"
#include "ui_p_pwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

p_pwindow::p_pwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::p_pwindow)
{
    ui->setupUi(this);

    game = new Game;
    //设置窗口大小
    this->setFixedSize(820, 780);
    this->move(400, 125);

}

p_pwindow::~p_pwindow()
{
    delete ui;
    delete game;
}
//初始化界面并调用游戏初始化函数
void p_pwindow::initgameinterface(bool ban){
    //是否是禁手模式
    game->startban = ban;
    banfrommain = ban;
    //开启监听
    setMouseTracking(true);
    //...
    //实现“选项”功能
        //悔棋
    connect(ui->action_undo, &QAction::triggered,
            [=](){
            //需要知道最新的落子点（应该在game里有记录）
            game->board[game->player1.newpoint.x()][game->player1.newpoint.y()] = game->player1.lastestinfor;
            game->board[game->player2.newpoint.x()][game->player2.newpoint.y()] = game->player2.lastestinfor;
            game->player1.newpoint.setX(game->player1.lastpoint.x());
            game->player1.newpoint.setY(game->player1.lastpoint.y());
            game->player2.newpoint.setX(game->player2.lastpoint.x());
            game->player2.newpoint.setY(game->player2.lastpoint.y());

            update();
    });
        //重新开始
    connect(ui->action_restart, &QAction::triggered,
            [=](){
            game->startgame(MAN_TO_MAN, banfrommain);
            QMessageBox::warning(this, "提示", "游戏重新开始");
            //初始化选项
            ui->action_undo->setEnabled(false);
            ui->action_restart->setEnabled(false);
            ui->action_dohint->setEnabled(true);
            ui->action_undohint->setEnabled(true);
            //开启监听
            setMouseTracking(true);
    });
    //初始化“禁手”选项
    connect(ui->action_dohint, &QAction::triggered,
            [=](){
            QMessageBox::warning(this, "提示", "你开启了禁手提示");
            //debug
            //qDebug() << "开启禁手提示了";
            game->banprompt = true;
            //label3.setText("禁手提示开启");
            //ui->statusbar->addWidget(&label3);
    });
    connect(ui->action_undohint, &QAction::triggered,
            [=](){
            QMessageBox::warning(this, "提示", "你关闭了禁手提示");
            game->banprompt = false;
            //label3.setText("禁手提示关闭");
            //ui->statusbar->addWidget(&label3);
    });
    //初始化返回菜单选项
    connect(ui->action_return, &QAction::triggered,
            [=](){
            setMouseTracking(false);
            emit ppsignal();
    });
//    //初始化状态栏
//    if(typefrommain == MAN_TO_MAN)
//        label1.setText("双人模式 ");
//    else if(typefrommain == MAN_TO_AI){
//        label1.setText("人机模式 ");
//    }
//    else{
//        label1.setText("机机模式 ");
//    }

//    if(banfrommain == true){
//        label2.setText("禁手模式");
//    }
//    else{
//        label2.setText("非禁手模式");
//    }
//    ui->statusbar->addWidget(&label1);
//    ui->statusbar->addWidget(&label2);
    //
    //调用游戏初始化函数
    game->startgame(typefrommain, banfrommain);
    //开始下棋了
}

void p_pwindow::chessbyperson()
{
    bool flag = false;
    if(clickPosCol != -1 && clickPosRow != -1 &&
            (game->board[clickPosRow][clickPosCol]==0||game->board[clickPosRow][clickPosCol]==4)){
        //考虑落子
        if(game->black_white){
            //放黑子记为1
            if(game->board[clickPosRow][clickPosCol]==4 && game->black_white)
                flag = true;            //下在禁手点位了
            game->player1.pointchange(clickPosRow, clickPosCol, game->board[clickPosRow][clickPosCol]);//记录该点位原先的信息
            game->board[clickPosRow][clickPosCol] = 1;
        }
        else{
            //放白子记为2
            game->player2.pointchange(clickPosRow, clickPosCol, game->board[clickPosRow][clickPosCol]);
            game->board[clickPosRow][clickPosCol] = 2;
        }
        update();
        //进行悔棋检查
        checkretract();
        //
        //判断输赢
        game->gamestate = game->win_lose(clickPosRow, clickPosCol, game->black_white);
        if(flag)
            game->gamestate = WHITEWIN;             //如果黑方下在禁手点位，白方获胜
        if(game->gamestate != NOWINNER){//胜负已分
            endgame(game->gamestate); //游戏结束
        }
        game->black_white = !(game->black_white);
    }
    else return;
}
//还需要处理结束后的棋盘
void p_pwindow::endgame(GameState state)
{
    if(state == BLACK_WIN){
        QMessageBox::about(this, "游戏结束","    黑方获胜    ");
        setMouseTracking(false);
        clickPosCol = -1;
        clickPosRow = -1;
    }
    else if(state == WHITEWIN){
        QMessageBox::about(this, "游戏结束","    白方获胜    ");
        setMouseTracking(false);
        clickPosCol = -1;
        clickPosRow = -1;
    }
    else{
        QMessageBox::about(this, "游戏结束","    和棋    ");
        setMouseTracking(false);
        clickPosCol = -1;
        clickPosRow = -1;
    }
}

void p_pwindow::checkretract()
{
    int num = 0;
    for(int i=0; i<=GRID; i++){
        for(int j=0; j<=GRID; j++){
            if(game->board[i][j] == 1 || game->board[i][j] == 2){
                num++;
            }
        }
    }
    if(num%2 || num == 0){
        ui->action_undo->setEnabled(false);
    }
    else{
        ui->action_undo->setEnabled(true);
    }
    if(num) ui->action_restart->setEnabled(true);
}



//可能需要多画一个框
void p_pwindow::paintEvent(QPaintEvent *)
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

    //画tracking点。不管有没有禁手，直接画。
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>=0 && clickPosRow<=GRID && clickPosCol>=0 &&
            clickPosCol<=GRID && hasMouseTracking()){
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
                    game->board[clickPosRow][clickPosCol] != 2){
                SelectPos = true;
            }
        }
        //右上角
        distance = sqrt((x-LeftTopX-INTERVEL)*(x-LeftTopX-INTERVEL)+(y-LeftTopY)*(y-LeftTopY));
        if(distance <= INDEX){
            clickPosCol = col + 1;
            clickPosRow = row;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != 2){
                SelectPos = true;
            }
        }
        //左下角
        distance = sqrt((x-LeftTopX)*(x-LeftTopX)+(y-LeftTopY-INTERVEL)*(y-LeftTopY-INTERVEL));
        if(distance <= INDEX){
            clickPosCol = col;
            clickPosRow = row + 1;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != 2){
                SelectPos = true;
            }
        }
        //右下角
        distance = sqrt((x-LeftTopX-INTERVEL)*(x-LeftTopX-INTERVEL)+(y-LeftTopY-INTERVEL)*(y-LeftTopY-INTERVEL));
        if(distance <= INDEX){
            clickPosCol = col + 1;
            clickPosRow = row + 1;
            if(game->board[clickPosRow][clickPosCol] != 1 ||
                    game->board[clickPosRow][clickPosCol] != 2){
                SelectPos = true;
            }
        }
        //完成，目标点的坐标已经存放在clickPosCol和clickPosRow中了
    }
    //之后要重绘（画tracking点）
    update();
}

//落完子后就要执行checkboard
void p_pwindow::mouseReleaseEvent(QMouseEvent *)
{
    if(SelectPos == false)  //只有选中了点击才有效果
        return;
    else SelectPos = false;

    if(hasMouseTracking() == false) return;

    if(game->startban)      //开启禁手模式就需要进行禁手的检查
        game->checkboard(game->board);
    if(clickPosRow != -1 && clickPosCol != -1 && game->board[clickPosRow][clickPosCol] != 1
            && game->board[clickPosRow][clickPosCol] != 2){
        QSound::play("E:\\yi\\programme\\qt\\personal programme\\chess.wav");
        //QTimer::singleShot(3000, this, SLOT(timeslot()));
        chessbyperson();
    }
    else return;
}

