#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QPainter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),win_pp(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //按钮设置
    button_man_man = new QPushButton("双人模式", this);
    button_man_ai = new QPushButton("人机模式", this);
    button_ai_ai = new QPushButton("机机模式", this);
    //按钮大小
    button_man_man->setGeometry(820, 90, 200, 100);
    button_man_ai->setGeometry(820, 260, 200, 100);
    button_ai_ai->setGeometry(820, 440, 200, 100);

    //按钮功能
    connect(button_man_man, &QPushButton::released,
            [=]()
            {
                int ret = QMessageBox::question(this, "Question", "是否选择黑子？");
                switch (ret) {
                case QMessageBox::Yes:
                    this->hide(),
                    win_pp.initgame(MAN_TO_MAN, true),
                    win_pp.show();
                    break;
                case QMessageBox::No:
                    this->hide(),
                    win_pp.initgame(MAN_TO_MAN, false);
                    win_pp.show();
                    break;
                default:
                    break;
                }

            });
    connect(button_man_ai, &QPushButton::released,
            [=](){
               int ret = QMessageBox::question(this, "Question", "是否选择黑子？");
              switch (ret) {
               case QMessageBox::Yes:
                    this->hide(),
                    win_pp.initgame(MAN_TO_AI, true),
                    win_pp.show();
                    break;
              case QMessageBox::No:
                    this->hide(),
                    win_pp.initgame(MAN_TO_AI, false);
                    win_pp.show();
                    break;
              default:
                  break;
               }
            });
    connect(button_ai_ai, &QPushButton::released, this, &MainWindow::AA_Slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AA_Slot()
{
    this->hide();
    win_pp.initgame(AI_TO_AI, true);
    win_pp.show();
}
