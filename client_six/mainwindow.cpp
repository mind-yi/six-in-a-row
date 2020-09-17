#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"

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
    //设置标题
    setWindowTitle("六子棋联机版");
    //选择联机模式就会触发此作用
    connect(ui->action_c, &QAction::triggered,
            [=](){
        this->hide();
        win_cs.show();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(), QPixmap("../boyi.png"));
}
