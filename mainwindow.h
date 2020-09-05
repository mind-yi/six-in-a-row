#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "p_pwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QPushButton *button_man_man;
    QPushButton *button_man_ai;
    QPushButton *button_ai_ai;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void PP_Slot();
    void PA_Slot();
    void AA_Slot();

private:
    Ui::MainWindow *ui;

    p_pwindow win_pp;   //新建一个子窗口：双人模式

};

#endif // MAINWINDOW_H
