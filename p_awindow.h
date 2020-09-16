#ifndef P_AWINDOW_H
#define P_AWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <Qsound>
#include <QTimer>


namespace Ui {
class p_awindow;
}

class p_awindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit p_awindow(QWidget *parent = 0);
    ~p_awindow();

public:
    Game *game;
    //GameType typefrommain;
    bool banfrommain = true;
    int clickPosRow = -1;
    int clickPosCol = -1;
    bool SelectPos = false;

public:
    void initgameinterface(bool ban);
    void who_chess();                           //解决谁先下棋的问题
    void chessbyperson();
    void endgame(GameState state);                            //游戏结束
    void checkretract();                        //顺便checkrestart
private slots:
    void on_button_retract_clicked();           //棋盘上的子是偶数才可以点击

    void on_button_restart_clicked();           //只要棋盘上有子，就可以点击

    void on_button_return_clicked();

    void on_button_start_clicked();

    void on_button_banhint_clicked();

protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
signals:
    void pasignal();
    void endsignal();
public slots:
    void AIturn();
private:
    Ui::p_awindow *ui;
};

#endif // P_AWINDOW_H
