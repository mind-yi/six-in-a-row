#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTcpServer>
#include <QTcpSocket>

#include "p_pwindow.h"
#include "p_awindow.h"
#include "a_awindow.h"
#include "c_swindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void dealSub();
    void dealEnabled();             //用来处理选项的可见性
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void on_button_startgame_clicked();
private:
    Ui::MainWindow *ui;
    GameType type = NOTYPE;     //初始化：开始并没有任何模式
    bool ban = true;
    p_pwindow win_pp;   //新建一个子窗口
    p_awindow win_pa;
    a_awindow win_aa;
    c_swindow win_cs;


};

#endif // MAINWINDOW_H
