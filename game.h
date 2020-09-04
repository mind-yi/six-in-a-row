#ifndef GAME_H
#define GAME_H

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

//禁手类型
//QString four_ban[18]={" Bbbb "," bbbB ",
//                      "wBbbb ","|Bbbb "," Bbbbw"," Bbbb|",
//                      "wbbbB ","|bbbB "," bbbBw"," bbbB|",
//                      "wBbbbw","wbbbBw","|Bbbb|","|bbbB|",
//                      "wBbbb|","wbbbB|","|Bbbbw","|bbbBw"};

enum GameType{
    MAN_TO_MAN,     //双人
    MAN_TO_AI,      //人机
    AI_TO_AI        //机机
};
enum GameState{
    PlAYING,        //开始游戏
    WIN,
    STALEMATE,      //和棋
    LOSE
};
class Game{

public:
    Game();           //构造函数
    void usergame();    //双人
    void compgame();    //人机对战
    void comp_comp();   //机机对战
    void startgame(GameType type);     //初始化

public:
    GameState gamestate;     //状态
    GameType gametype;      //游戏模式
    int four_ban, five_ban; //用于禁手
    bool startban;          //是否开启禁手
    bool banprompt;          //是否开启禁手提示
    bool black_white = 1;      //true:黑
    /*记录棋子的情况
     *1:黑子
     *-1:白子
     *0:空位
     *4:四四禁手点位
     *5:五五禁手点位
     */
    int board[21][21];
    /*记录每个点位的得分情况（用于AI参与的情况）
     *
     */
    int board_score[21][21];
};
#endif // GAME_H
//int Game::BOARD = 700;
//int Game::GRID = 20;
//int Game::CHESS_SIZE = 15;
//int Game::CHESS_MATE = 5;
//int Game::INTERVEL = BOARD/20;
//int Game::INDEX = INTERVEL*0.4;
//int Game::AI_THINK = 800;

