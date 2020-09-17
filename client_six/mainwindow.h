#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "c_swindow.h"

#include <QMainWindow>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTcpSocket>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintEvent(QPaintEvent *);
public slots:

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;
    c_swindow win_cs;
};

#endif // MAINWINDOW_H
