#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process_code = new QProcess;
    path = "code";
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QUrl url("https://marketplace.visualstudio.com/items?itemName=MS-CEINTL.vscode-language-pack-zh-hans");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_3_clicked()
{
    QByteArray username = qgetenv("USERNAME");
    path = QString(username).toStdString();
    path = "C:\\users\\" + path + "\\AppData\\Local\\Programs\\Microsoft VS Code\\";
    qDebug() << path;
    if (QFile::exists((path + "Code.exe").c_str())) {
        // 找到应用
        return;
    } else {
        // 未找到，尝试在D盘寻找
        path = "D:\\Microsoft VS Code\\";
        qDebug() << path;
        if (QFile::exists((path + "Code.exe").c_str())) {
            // 找到应用
            return;
        }
        else{
            qDebug() << "open code failed";
            return;
        }
    }
}

