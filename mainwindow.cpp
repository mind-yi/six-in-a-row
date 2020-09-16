#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //建立窗口的连接
    connect(&win_pa, SIGNAL(pasignal()), this, SLOT(dealSub()));
    connect(&win_aa, SIGNAL(aasignal()), this, SLOT(dealSub()));
    connect(&win_pp, SIGNAL(ppsignal()), this, SLOT(dealSub()));
    //选项的可见性设置
    ui->menu_forbid->setEnabled(false);
    ui->menu_chesschoice->setEnabled(false);

    //游戏模式
    connect(ui->action_m_m, &QAction::triggered,
            [=]()
            {
                ui->menu_chesschoice->setEnabled(true);
                ui->menu_forbid->setEnabled(true);
                type = MAN_TO_MAN;
                ban = true;             //初始化
                ui->action_m_m->setEnabled(false);
                ui->action_a_a->setEnabled(true);
                ui->action_m_a->setEnabled(true);
                //cs
                ui->action_c_s->setEnabled(true);

                ui->action_forbid->setEnabled(true);
                ui->action_unforbid->setEnabled(true);

                ui->action_black->setEnabled(true);
                ui->action_white->setEnabled(true);
            });
    connect(ui->action_m_a, &QAction::triggered,
            [=](){
                ui->menu_chesschoice->setEnabled(true);
                ui->menu_forbid->setEnabled(true);
                type = MAN_TO_AI;
                ban = true;
                ui->action_m_a->setEnabled(false);
                ui->action_a_a->setEnabled(true);
                ui->action_m_m->setEnabled(true);
                //cs
                ui->action_c_s->setEnabled(true);

                ui->action_forbid->setEnabled(true);
                ui->action_unforbid->setEnabled(true);

                ui->action_black->setEnabled(true);
                ui->action_white->setEnabled(true);
            }
            );
    connect(ui->action_a_a, &QAction::triggered,
            [=](){
                ui->menu_chesschoice->setEnabled(false);
                ui->menu_forbid->setEnabled(true);
                type = AI_TO_AI;
                ban = true;
                ui->action_a_a->setEnabled(false);
                ui->action_m_m->setEnabled(true);
                ui->action_m_a->setEnabled(true);
                //c_s
                ui->action_c_s->setEnabled(true);

                ui->action_forbid->setEnabled(true);
                ui->action_unforbid->setEnabled(true);

                ui->action_black->setEnabled(true);
                ui->action_white->setEnabled(true);
            });
        //特殊：联机模式 cs
    connect(ui->action_c_s, &QAction::triggered,
            [=](){
                this->hide();
                win_cs.show();
    });
    //禁手设置
    connect(ui->action_forbid, &QAction::triggered,
            [=](){
                ban = true;
                ui->action_forbid->setEnabled(false);
                ui->action_unforbid->setEnabled(true);
            });
    connect(ui->action_unforbid, &QAction::triggered,
            [=](){
                ban = false;
                ui->action_unforbid->setEnabled(false);
                ui->action_forbid->setEnabled(true);
            });
    //棋类选择
    connect(ui->action_black, &QAction::triggered,
            [=](){
                if(type == MAN_TO_AI){
                    win_pa.game->player1.setnametype(1);
                    win_pa.game->player2.setnametype(0);

                    ui->action_black->setEnabled(false);
                    ui->action_white->setEnabled(true);
                }
            });
    connect(ui->action_white, &QAction::triggered,
            [=](){
                if(type == MAN_TO_AI){
                   win_pa.game->player1.setnametype(0);
                   win_pa.game->player2.setnametype(1);

                   ui->action_black->setEnabled(true);
                   ui->action_white->setEnabled(false);
                }
            });



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dealSub()
{
    //返回菜单后，原先的游戏棋盘信息清零，玩家信息清零
    if(type == MAN_TO_MAN){
        win_pp.game->closegame();
    }
    else if(type == MAN_TO_AI){
        win_pa.game->closegame();
    }
    else if(type == AI_TO_AI){
        win_aa.game->closegame();
    }
    //窗口的显示
    if(type == MAN_TO_MAN)
        win_pp.hide();
    else if(type == MAN_TO_AI){
        win_pa.hide();
    }
    else if(type == AI_TO_AI){
        win_aa.hide();
    }
    //选项的可选性归位设置
    type = NOTYPE;
    ui->action_m_m->setEnabled(true);
    ui->action_a_a->setEnabled(true);
    ui->action_m_a->setEnabled(true);
        //cs
    ui->action_c_s->setEnabled(true);
    ui->action_black->setEnabled(true);
    ui->action_white->setEnabled(true);
    ui->action_forbid->setEnabled(true);
    ui->action_unforbid->setEnabled(true);
        //菜单上只有游戏模式能选
    ui->menu_forbid->setEnabled(false);
    ui->menu_chesschoice->setEnabled(false);

    this->show();
}

void MainWindow::dealEnabled()
{
    //因为先选的是类型，所以应该先判断类型
    if(type == MAN_TO_AI || type == MAN_TO_MAN){
        ui->menu_forbid->setEnabled(true);
        ui->menu_chesschoice->setEnabled(true);
        if(type == MAN_TO_MAN){
            ui->action_m_m->setEnabled(false);
            ui->action_m_a->setEnabled(true);
            ui->action_a_a->setEnabled(true);
        }
        else{
            ui->action_m_a->setEnabled(false);
            ui->action_m_m->setEnabled(true);
            ui->action_a_a->setEnabled(true);
        }
    }
    else if(type == AI_TO_AI){
        ui->menu_forbid->setEnabled(true);

        ui->action_a_a->setEnabled(false);
        ui->action_m_a->setEnabled(true);
        ui->action_m_m->setEnabled(true);
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(), QPixmap("../boyi.png"));
}

void MainWindow::on_button_startgame_clicked()
{
    if(type == NOTYPE){
        QMessageBox::warning(this, "提示", "请对游戏进行必要的选择");
        return;
    }
    else{
//        if(type == MAN_TO_MAN && (win_pp.game->player1.getnametype() == -1 || win_pp.game->player2.getnametype() == -1))
//        {
//            QMessageBox::warning(this, "提示", "请对游戏进行必要的选择");
//            return;
//        }
        if(type == MAN_TO_AI && (win_pa.game->player1.getnametype() == -1 || win_pa.game->player2.getnametype() == -1))
        {
            QMessageBox::warning(this, "提示", "请对游戏进行必要的选择");
            return;
        }
    }
    this->hide();
    if(type == MAN_TO_MAN){
        win_pp.initgameinterface(ban);
        win_pp.show();
    }
    else if(type == MAN_TO_AI){
        win_pa.initgameinterface(ban);
        win_pa.show();
    }
    else{
        win_aa.initgameinterface(ban);
        win_aa.show();
    }
}
