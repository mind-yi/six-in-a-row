#ifndef GAME_H
#define GAME_H
//https://blog.csdn.net/pige666/article/details/106378477?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param
//（五子棋基本知识）
#include "gameplayer.h"
#include <QString>
#include <QPoint>
#include <QList>
#include <QSound>
const int BOARD = 700;  //棋盘的大小
const int GRID = 20;    //格子的个数
const int CHESS_SIZE = 15;  //棋子的半径
const int CHESS_MATE = 2;   //棋子光标半径
const int PIX_SIZE = 10;    //图片半径
const int MARGIN = 40;      //边缘
const int INTERVEL = BOARD/20;  //棋子间隔
const int INDEX = INTERVEL*0.4;  //索引范围

const int bestnum = 5;      //最优点的个数
const int AI_THINK = 800;   //AI思考时间

#define INTMAX 1073741824

#define OTHER 0
#define FREE1 1     //白
#define free1 2     //黑
#define BLOCK2 3
#define block2 4
#define FREE2 5
#define free2 6
#define BLOCK3 7
#define block3 8
#define FREE3 9
#define free3 10
#define BLOCK4 11
#define block4 12
#define FREE4 13
#define free4 14
#define BLOCK5 15
#define block5 16
#define FREE5 17
#define free5 18
#define WWIN 19      //白赢
#define BWIN 20

enum GameType{
    MAN_TO_MAN,     //双人
    MAN_TO_AI,      //人机
    AI_TO_AI,        //机机
    NOTYPE          //没有模式
};
enum GameState{
    STALEMATE,      //和棋
    BLACK_WIN,
    WHITEWIN,
    NOWINNER
};
struct EVALUATION
{
    int score;          //评估的分数
    GameState result;
    int STATE[21];      //共21种状态
};
struct POINT
{
    QPoint pos[bestnum];     //bestnum个优先点的坐标
    int cnt;
    int score[bestnum];
};
struct DECISION
{
    QPoint pos;
    int evalscore;
};

class Game{

public:
    Game();           //构造函数
    void startgame(GameType type, bool ban);
    void closegame();
    GameState win_lose(int row, int col);   //判断输赢
    void checkboard(int (*b)[GRID+1]);          //检查棋盘
    void processforbid(QString &str); //处理禁手
    void initai();
    EVALUATION evaluate(int (*b)[21]);        //评估函数:对board的空位进行分值评估
    POINT seekpoint(int (*b)[21], bool chesstype);          //AI模式调用
    int analyse(int (*b)[21], int depth, int alpha, int beta);  //AI模式
    void copyBoard(int (*s)[21], int (*d)[21]);             //AI模式调用
    GameState chessbyai();           //区别于chessbyperson,是AI下棋了
    //QList<QPoint> killpoint(int (*b)[GRID+1]);      //找到杀棋点
    //bool kill_analyse(int (*b)[GRID+1], int depth);
    bool seekwinpoint(int (*b)[GRID+1]);
public:
    GameState gamestate;     //状态
    GameType gametype;      //游戏模式
    int four_ban, five_ban; //用于禁手
    bool startban;          //是否开启禁手
    bool banprompt;          //是否开启禁手提示
    bool black_white = true;      //true:黑(都是黑方先下棋)
    int boardban[21][21];   //用于禁手情况
    /*记录棋子的情况
     *1:黑子
     *-1:白子
     *0:空位
     */
    int board[21][21];
    int typeall[5][5][5][5][5][5][5];        //记录所有的类型
    /*记录每个点位的得分情况（用于AI参与的情况）
     *
     */
    int board_score[21][21];
    DECISION decision;

    GamePlayer player1;
    GamePlayer player2;
    //记录上一次的位置信息
    int lastinfo1;
    int lastinfo2;
    //目前没有悔棋的功能（因为没有上一步棋盘的信息）
};
#endif // GAME_H

