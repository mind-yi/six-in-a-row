#ifndef GAME_H
#define GAME_H
//https://blog.csdn.net/pige666/article/details/106378477?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.channel_param
//（五子棋基本知识）
#include <QString>
const int BOARD = 700;  //棋盘的大小
const int GRID = 20;    //格子的个数
const int CHESS_SIZE = 15;  //棋子的半径
const int CHESS_MATE = 2;   //棋子光标半径
const int PIX_SIZE = 10;    //图片半径
const int MARGIN = 40;      //边缘
const int INTERVEL = BOARD/20;  //棋子间隔
const int INDEX = INTERVEL*0.4;  //索引范围
const int AI_THINK = 800;   //AI思考时间

enum GameType{
    MAN_TO_MAN,     //双人
    MAN_TO_AI,      //人机
    AI_TO_AI        //机机
};
enum GameState{
    PlAYING,        //开始游戏
    STALEMATE,      //和棋

    BLACK_WIN,
    BLACK_LOSE,
    WHITEWIN,
    WHITELOSE,
    NOWINNER
};
class Game{

public:
    Game();           //构造函数
    void usergame();    //双人
    void compgame();    //人机对战
    void comp_comp();   //机机对战
    void startgame(GameType type);     //初始化
    GameState win_lose(int row, int col);   //判断输赢
    void checkboard();          //检查棋盘
    void processforbid(QString &str, int col, int row); //处理禁手
    void chessbyai(QString &str, int row, int col);       //检查每条连线的可下性，checkboard()调用

public:
    GameState gamestate;     //状态
    GameType gametype;      //游戏模式
    int four_ban, five_ban; //用于禁手
    bool startban;          //是否开启禁手
    bool banprompt;          //是否开启禁手提示
    bool black_white = 1;      //true:黑
    int boardban[21][21];   //用于禁手情况
    /*记录棋子的情况
     *1:黑子
     *-1:白子
     *0:空位
     */
    int board[21][21];
    /*记录每个点位的得分情况（用于AI参与的情况）
     *
     */
    int board_score[21][21];
    //目前没有悔棋的功能（因为没有上一步棋盘的信息）
};
#endif // GAME_H

