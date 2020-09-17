#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <QPoint>
class GamePlayer
{
private:
    bool chesschoic;            //true:黑方
    int nametype;              //true:人，false:AI，-1:没有选择身份
public:
    bool choic_not = false;     //是否进行了棋类的选择
    QPoint newpoint;            //刚下的点位
    QPoint lastpoint;           //上次的点位
    int lastestinfor;              //该点没落子前的点位信息
    //悔棋的实现
    /*
     * 对于人机双方应该都下了一遍后才有悔棋按钮才是enable的（落子为偶数）
     * 棋盘上没有落子的时候时不会有悔棋功能的
     * 棋盘上有落子的时候需要检查落子的情况
*/
public:
    GamePlayer();
    void setchesschoic(int chess);
    void setnametype(int b);
    int getnametype();
    void pointchange(int row, int col, int info);
    void closeplayer();
};

#endif // GAMEPLAYER_H
