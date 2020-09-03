#ifndef P_PWINDOW_H
#define P_PWINDOW_H

#include "game.h"
#include <QMainWindow>


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
private:
    Ui::p_pwindow *ui;
};

#endif // P_PWINDOW_H
