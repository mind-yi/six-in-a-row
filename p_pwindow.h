#ifndef P_PWINDOW_H
#define P_PWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <Qsound>
#include <QTimer>

#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class p_pwindow;
}

class p_pwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit p_pwindow(QWidget *parent = 0);
    ~p_pwindow();
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);    //不懂event
    void mouseReleaseEvent(QMouseEvent *);
public:
    void initgameinterface(bool ban);       //仅初始化游戏的界面
    void chessbyperson();
    void endgame(GameState state);
    void checkretract();                        //顺便checkrestart
public:
    QLabel label1;
    QLabel label2;
    QLabel label3;
//记录来自主窗口的信息
public:
    GameType typefrommain;
    bool banfrommain = true;    //默认开启

    Game *game;
signals:
    void ppsignal();
public slots:

private:
    Ui::p_pwindow *ui;
    int clickPosRow, clickPosCol;   //储存将点击的位置。
    bool SelectPos = false;         //该点是否能选
};

#endif // P_PWINDOW_H
