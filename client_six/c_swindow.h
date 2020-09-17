#ifndef C_SWINDOW_H
#define C_SWINDOW_H

#include "game.h"

#include <QMainWindow>
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
class c_swindow;
}

class c_swindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit c_swindow(QWidget *parent = 0);
    ~c_swindow();

    void proccessinfo(QString str);
    void chessbyperson();
    void endgame(GameState state);
public:
    Game *game;
    bool ban;
    bool your_chess;
    bool do_chess = false;          //没有落子
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void on_button_connect_clicked();

private:
    Ui::c_swindow *ui;
    QTcpSocket *tcpSocket;
    int clickPosRow, clickPosCol;   //储存将点击的位置。
    bool SelectPos = false;         //该点是否能选

};

#endif // C_SWINDOW_H
