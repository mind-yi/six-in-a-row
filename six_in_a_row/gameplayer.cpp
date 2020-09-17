#include "gameplayer.h"

GamePlayer::GamePlayer()
{
    newpoint.setX(-1);
    newpoint.setY(-1);
    lastpoint.setX(-1);
    lastpoint.setY(-1);
    lastestinfor = -1;         //没有信息
    //初始化玩家身份
    nametype = -1;          //没有身份
}

void GamePlayer::setchesschoic(int chess)
{
    if(chess)
        chesschoic = true;
    else chesschoic = false;
}

void GamePlayer::setnametype(int b)
{
    if(b) nametype = 1;
    else nametype = 0;
}

int GamePlayer::getnametype()
{
    return nametype;
}

void GamePlayer::pointchange(int row, int col, int info)
{
    lastpoint.setX(newpoint.x());
    lastpoint.setY(newpoint.y());
    newpoint.setX(row);
    newpoint.setY(col);
    lastestinfor = info;
}

void GamePlayer::closeplayer()
{
    newpoint.setX(-1);
    newpoint.setY(-1);
    lastpoint.setX(-1);
    lastpoint.setY(-1);
    lastestinfor = -1;         //没有信息

    nametype = -1;          //没有身份
}
