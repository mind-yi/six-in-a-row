#ifndef GAME_H
#define GAME_H

const int BOARD = 700;  //棋盘的大小
const int GRID = 20;    //格子的个数
const int CHESS_SIZE = 15;  //棋子的半径
const int CHESS_MATE = 5;   //棋子光标半径
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
    WIN,
    STALEMATE,      //和棋
    LOSE
};
class Game{

public:
    Game();         //构造函数
    void usergame();    //双人
    void compgame();    //人机对战
    void comp_comp();   //机机对战

public:
    bool state;     //状态
    int board[20][20];  //棋子位置

};
#endif // GAME_H
//int Game::BOARD = 700;
//int Game::GRID = 20;
//int Game::CHESS_SIZE = 15;
//int Game::CHESS_MATE = 5;
//int Game::INTERVEL = BOARD/20;
//int Game::INDEX = INTERVEL*0.4;
//int Game::AI_THINK = 800;

