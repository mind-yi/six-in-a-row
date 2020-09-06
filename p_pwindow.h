#ifndef P_PWINDOW_H
#define P_PWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>


namespace Ui {
class p_pwindow;
}

class p_pwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit p_pwindow(QWidget *parent = 0);
    ~p_pwindow();

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);    //不懂event
    void mouseReleaseEvent(QMouseEvent *event);
public:
    void initgame(GameType type, bool chess);
    void chessbyperson();
    void endgame(GameState state);
public:
    QPushButton *button_retract;
    QPushButton *button_return;
    QPushButton *button_stalemant;
    QPushButton *button_restart;
    QPushButton *button_stop;
    QPushButton *button_ban;
private:
    Ui::p_pwindow *ui;

    Game *game;

    int clickPosRow, clickPosCol;   //储存将点击的位置。
    bool SelectPos = false;         //该点是否能选
};

#endif // P_PWINDOW_H
