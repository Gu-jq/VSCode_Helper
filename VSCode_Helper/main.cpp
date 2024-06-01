#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("* { color: rgb(0, 0, 0); }");
    MainWindow w;
    w.setFixedSize(QSize(480, 480));
    w.setWindowTitle("VS Code 小帮手");
    w.show();
    return a.exec();
}
