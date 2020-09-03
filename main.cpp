#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setFixedSize(1100, 780);
    w.setWindowTitle("六子棋");


    return a.exec();
}
