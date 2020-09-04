#include "game.h"

Game::Game()
{

}


void Game::startgame(GameType type)
{
    //棋盘上的所有点位清零
    for(int i=0; i<21; i++){
        for(int j=0; j<21; j++){
            board[i][j] = 0;
            board_score[i][j] = 0;
        }
    }
    //禁手归零
    four_ban = 0;
    five_ban = 0;
    banprompt = false;  //默认关闭禁手
    startban = true;    //默认开启禁手
    //各种模式
    gametype = type;
}
