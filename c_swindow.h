#ifndef C_SWINDOW_H
#define C_SWINDOW_H

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
class c_swindow;
}

class c_swindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit c_swindow(QWidget *parent = 0);
    ~c_swindow();
public:
    Game *game;
    bool ban;
    bool your_chess;        //你选择的棋类
public:
    void proccessinfo(QString str);
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    Ui::c_swindow *ui;
    int clickPosRow, clickPosCol;   //储存将点击的位置。
    bool SelectPos = false;         //该点是否能选

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
};

#endif // C_SWINDOW_H
