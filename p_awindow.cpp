#include "p_awindow.h"
#include "ui_p_awindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>       //计算两点间的距离
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
p_awindow::p_awindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::p_awindow)
{
    ui->setupUi(this);

    game = new Game;
}

p_awindow::~p_awindow()
{
    delete ui;
    delete game;
}

void p_awindow::initgameinterface(bool ban)
{
    banfrommain = ban;
    //初始化游戏
    game->startgame(MAN_TO_AI, ban);
    //默认关闭监听
    setMouseTracking(false);
    //初始化按钮
    if(ban == 0){
        ui->button_banhint->hide();
    }
    ui->button_start->setEnabled(true);
    ui->button_restart->setEnabled(false);
    ui->button_retract->setEnabled(false);
    ui->button_return->setEnabled(true);
}

void p_awindow::who_chess()
{
    if(game->player1.getnametype() == 1){                //人先下
        setMouseTracking(true);
    }
    else{
        GameState state;
        setMouseTracking(false);                    //机器先下
        state = game->chessbyai();
        //AI落完子后,要update()
        update();
        //进行悔棋检查
        checkretract();
        if(state == BLACK_WIN || state == WHITEWIN){
            endgame(state);
        }
        setMouseTracking(true);                     //该人下
    }
}

void p_awindow::chessbyperson()
{
    bool flag = false;
    if(clickPosCol != -1 && clickPosRow != -1 &&
            (game->board[clickPosRow][clickPosCol]==0||game->board[clickPosRow][clickPosCol]==4)){
        //考虑落子
        //！！！
        //关闭监视
        setMouseTracking(false);
        if(game->startban)                  //开启禁手模式就需要进行禁手的检查
            game->checkboard(game->board);
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
        //判断输赢
        game->gamestate = game->win_lose(clickPosRow, clickPosCol);
        if(flag)
            game->gamestate = WHITEWIN;             //如果黑方下在禁手点位，白方获胜
        if(game->gamestate != NOWINNER){//胜负已分
            endgame(game->gamestate); //游戏结束
        }
        game->black_white = !(game->black_white);
    }
    else return;
}


void p_awindow::on_button_retract_clicked()
{
    game->board[game->player1.newpoint.x()][game->player1.newpoint.y()] = game->player1.lastestinfor;
    game->board[game->player2.newpoint.x()][game->player2.newpoint.y()] = game->player2.lastestinfor;
    game->player1.newpoint.setX(game->player1.lastpoint.x());
    game->player1.newpoint.setY(game->player1.lastpoint.y());
    game->player2.newpoint.setX(game->player2.lastpoint.x());
    game->player2.newpoint.setY(game->player2.lastpoint.y());
    //悔棋不能再点了
    ui->button_retract->setEnabled(false);
    update();
}

void p_awindow::on_button_restart_clicked()
{
    game->startgame(MAN_TO_AI, banfrommain);
    QMessageBox::warning(this, "提示", "游戏重新开始");
    //开始下第一手棋
    ui->button_retract->setEnabled(false);
    ui->button_restart->setEnabled(false);
    ui->button_start->setEnabled(true);
    //像界面初始化那样，默认关闭监听
    setMouseTracking(false);
}

void p_awindow::on_button_return_clicked()
{
    setMouseTracking(false);                //监听关闭
    emit pasignal();
}

void p_awindow::on_button_start_clicked()
{
    //点击后就不能用了
    ui->button_start->setEnabled(false);
    //第一步谁先下呢？
    who_chess();
}

void p_awindow::paintEvent(QPaintEvent *e)
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

void p_awindow::mouseMoveEvent(QMouseEvent *event)
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

void p_awindow::mouseReleaseEvent(QMouseEvent *)
{
    if(SelectPos == false)  //只有选中了点击才有效果
        return;
    else SelectPos = false;
    //！！！
    if(hasMouseTracking() == false) return;         //如过没有开启监视（肯定是AI下）那么即便点击也不能监视
    if(clickPosRow != -1 && clickPosCol != -1 && game->board[clickPosRow][clickPosCol] != 1
            && game->board[clickPosRow][clickPosCol] != 2){
        QSound::play("E:\\yi\\programme\\qt\\personal programme\\chess.wav");
        chessbyperson();
    }
    else return;
    //人下完了，该AI了
    QTimer::singleShot(1000, this, SLOT(AIturn()));
}


void p_awindow::endgame(GameState state)
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

void p_awindow::checkretract()
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
        ui->button_retract->setEnabled(false);
    }
    else{
        ui->button_retract->setEnabled(true);
    }
    if(num) ui->button_restart->setEnabled(true);
}

void p_awindow::AIturn()
{
    //如果游戏有胜负了，AI就没有必要再下
    if(game->gamestate != NOWINNER){
        return;
    }
    GameState state;
    setMouseTracking(false);
    state = game->chessbyai();
    //AI下完后要update()，否则不会直接画出AI落点的位置
    update();
    //进行悔棋检查
    checkretract();
    if(state == WHITEWIN || state == BLACK_WIN){
        endgame(state);
    }
    setMouseTracking(true);
}

void p_awindow::on_button_banhint_clicked()
{
    if(ui->button_banhint->text() == "开启禁手提示"){
        game->banprompt = true;
        ui->button_banhint->setText("关闭禁手提示");
    }
    else{
        game->banprompt = false;
        ui->button_banhint->setText("开启禁手提示");
    }
}
