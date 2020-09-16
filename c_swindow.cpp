#include "c_swindow.h"
#include "ui_c_swindow.h"

#include "game.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QHostAddress>

c_swindow::c_swindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::c_swindow)
{
    ui->setupUi(this);

    game = new Game;

    //初始化鼠标监听
    setMouseTracking(false);

    // ////////////////////////////联机部分/////////////////////////////

    tcpServer = NULL;
    tcpSocket = NULL;

    //监听套接字，指定父对象，让其自动回收空间
    tcpServer = new QTcpServer(this);

    tcpServer->listen(QHostAddress::Any, 1024);     //端口是1024

    connect(tcpServer, &QTcpServer::newConnection,
            [=](){
            //取出建立好连接的套接字
            tcpSocket = tcpServer->nextPendingConnection();

            //获取对方的IP和端口
            QString ip = tcpSocket->peerAddress().toString();
            qint16 port = tcpSocket->peerPort();
            QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);

            QMessageBox::information(this, "提示", temp);
            //读取客户端传递的信息（这一部分会一直有响应）
            connect(tcpSocket, &QTcpSocket::readyRead,
                    [=](){
                    //从通信套接字中取出内容
                    QByteArray array = tcpSocket->readAll();
                    QString str = array.toStdString();
                    //处理过来的信息
                    proccessinfo(str);
            });
    });
    // ///////////////////////////////////////////////////////////////////////
}

c_swindow::~c_swindow()
{
    delete ui;
    delete game;
}

void c_swindow::proccessinfo(QString str){
    if(str.contains("chesstype##")){
        char c = str[11];
        if(c-48 == 1){
            game->black_white = 0;
            your_chess = 0;
            QMessageBox::information(this, "提示", "对方选黑棋");
        }
        else{
            game->black_white = 1;
            your_chess = 1;
            QMessageBox::information(this, "提示", "对方选白棋");
        }
    }
    else if(str.contains("bantype##")){
        char c = str[9];
        if(c-48 == 1){
            game->startban = true;
            ban = true;
            game->startgame(MAN_TO_MAN, true);
            QMessageBox::information(this, "提示", "禁手模式");
        }
        else{
            game->startban = false;
            ban = false;
            game->startgame(MAN_TO_MAN, false);
            QMessageBox::information(this, "提示", "非禁手模式");
        }
        //可以开始游戏了
        setMouseTracking(true);
    }
    //最后保证你选择的棋类不被初始化改变
    game->black_white = your_chess;


}

void c_swindow::paintEvent(QPaintEvent *event)
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

void c_swindow::mouseMoveEvent(QMouseEvent *event)
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
