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
    QByteArray username = qgetenv("USERNAME");
    path = QString(username).toStdString();
    path = "C:\\users\\" + path + "\\AppData\\Local\\Programs\\Microsoft VS Code\\";
    if (QFile::exists((path + "Code.exe").c_str())) {
        // 找到
        ui->stackedWidget->setCurrentIndex(2);
        return;
    } else {
        // 未找到，尝试在D盘寻找
        path = "X:\\Microsoft VS Code\\"; //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
        if (QFile::exists((path + "Code.exe").c_str())) {
            // 找到
            ui->stackedWidget->setCurrentIndex(2);
            return;
        }
        else{
            ui->stackedWidget->setCurrentIndex(1);
            ui->notice_1->setText(QString(" "));
            ui->cnext_button_1->hide();
            return;
        }
    }

}

void MainWindow::on_pushButton_9_clicked()
{
    QUrl url("https://code.visualstudio.com/Download");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_10_clicked()
{
    QByteArray username = qgetenv("USERNAME");
    path = QString(username).toStdString();
    path = "C:\\users\\" + path + "\\AppData\\Local\\Programs\\Microsoft VS Code\\";
    if (QFile::exists((path + "Code.exe").c_str())) {
        // 找到
        ui->notice_1->setText(QString("成功找到VS Code"));
        ui->cnext_button_1->show();
    } else {
        // 未找到，尝试在D盘寻找
        path = "X:\\Microsoft VS Code\\"; //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
        if (QFile::exists((path + "Code.exe").c_str())) {
            // 找到
            ui->notice_1->setText(QString("成功找到VS Code"));
            ui->cnext_button_1->show();
        }
        else{
            path = ui->path_input->text().toStdString();
            qDebug() << path;
            if (QFile::exists((path + "Code.exe").c_str())) {
                // 找到
                ui->notice_1->setText(QString("成功找到VS Code"));
                ui->cnext_button_1->show();
            }
            else if(QFile::exists((path + "\\Code.exe").c_str())){
                // 找到
                ui->notice_1->setText(QString("成功找到VS Code"));
                ui->cnext_button_1->show();
            }
            else{
                ui->notice_1->setText(QString("未找到，请检查是否安装及输入路径正确性"));
            }
        }
    }
}


void MainWindow::on_cnext_button_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

