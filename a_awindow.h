#ifndef A_AWINDOW_H
#define A_AWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <Qsound>
#include <QTimer>

namespace Ui {
class a_awindow;
}

class a_awindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit a_awindow(QWidget *parent = 0);
    ~a_awindow();
public:
    Game *game;
    //GameType typefrommain;
    bool banfrommain = true;

public:
    void initgameinterface(bool ban);
    void endgame(GameState state);
protected:
    void paintEvent(QPaintEvent *event);

signals:
    void aasignal();
private slots:
    void on_button_start_clicked();

    void on_button_restart_clicked();

    void on_button_return_clicked();

private:
    Ui::a_awindow *ui;
};

#endif // A_AWINDOW_H
