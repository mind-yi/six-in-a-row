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

    //设置标题
    setWindowTitle("六子棋联机模式");

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
            ui->lineEditIP->setText(ip);
            QMessageBox::information(this, "提示", temp);
            //读取客户端传递的信息（这一部分会一直有响应）
            connect(tcpSocket, &QTcpSocket::readyRead,
                    [=](){
                    //从通信套接字中取出内容
                    QByteArray array = tcpSocket->readAll();
                    QString str = QString(array);
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
void c_swindow::chessbyperson(){
    bool flag = false;
    if(clickPosCol != -1 && clickPosRow != -1 &&
            (game->board[clickPosRow][clickPosCol]==0||game->board[clickPosRow][clickPosCol]==4)){
        //考虑落子
        if(game->black_white){
            //放黑子记为1
            if(game->board[clickPosRow][clickPosCol]==4 && game->black_white)
                flag = true;            //下在禁手点位了
//            game->player1.pointchange(clickPosRow, clickPosCol, game->board[clickPosRow][clickPosCol]);//记录该点位原先的信息
            game->board[clickPosRow][clickPosCol] = 1;
        }
        else{
            //放白子记为2
//            game->player2.pointchange(clickPosRow, clickPosCol, game->board[clickPosRow][clickPosCol]);
            game->board[clickPosRow][clickPosCol] = 2;
        }
        update();
        //落子后，手里自然没有子了
        do_chess = false;
        //进行悔棋检查
        //checkretract();
        //向客户传递下棋信息
        tcpSocket->write(QString("%1###%2").arg(clickPosRow).arg(clickPosCol).toUtf8());
        //判断输赢
        game->gamestate = game->win_lose(clickPosRow, clickPosCol, game->black_white);
        if(flag)
            game->gamestate = WHITEWIN;             //如果黑方下在禁手点位，白方获胜
        if(game->gamestate != NOWINNER){//胜负已分
            endgame(game->gamestate); //游戏结束
        }
        //game->black_white = !(game->black_white);
    }
    else return;
}

void c_swindow::proccessinfo(QString str){
    if(str.contains("chesstype##")){
        int t = str.mid(11).toInt();
        if(t == 1){
            game->black_white = 0;
            your_chess = 0;
            QMessageBox::information(this, "提示", "对方选黑棋");
        }
        else{
            game->black_white = 1;
            your_chess = 1;
            //黑方先拿到子
            do_chess = true;
            QMessageBox::information(this, "提示", "对方选白棋");
        }
    }
    else if(str.contains("bantype##")){
        int t = str.mid(9).toInt();
        if(t == 1){
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

    if(str.contains("###")){
        //把里面的数字取出来
        int row = str.section("###", 0, 0).toInt();
        int col = str.section("###", 1, 1).toInt();
        if(game->black_white){      //记录对方的下棋信息
            game->board[row][col] = 2;
        }
        else{
            game->board[row][col] = 1;
        }
        //再对棋盘进行一次检查
        game->gamestate = game->win_lose(row, col, !game->black_white);
        if(game->gamestate == BLACK_WIN || game->gamestate == WHITEWIN){
            endgame(game->gamestate);
        }
        //对方下完后，己方手里就有子了
        do_chess = true;
        //重绘
        update();
    }
}

void c_swindow::endgame(GameState state)
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

void c_swindow::paintEvent(QPaintEvent *)
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

void c_swindow::mouseReleaseEvent(QMouseEvent *){
    if(SelectPos == false)  //只有选中了点击才有效果
        return;
    else SelectPos = false;

    if(hasMouseTracking() == false) return;

    //如果手里没有棋子，自然不能下
    if(do_chess == false){
        QMessageBox::warning(this, "提示", "请等待对方落子");
        return;
    }

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
