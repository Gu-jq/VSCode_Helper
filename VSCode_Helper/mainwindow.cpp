#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include "string_operator.h"
#include <QDir>
#include "json_operator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process_code = new QProcess;
}


MainWindow::~MainWindow()
{
    delete ui;
}


int path_check(std::string aim_array[], int aim_num, std::string path_array[], int path_num){
    for(int i = 0; i < path_num; i++){
        int cnt = 0;
        for (int j = 0; j < aim_num; j++){
            if(QFile::exists((path_array[i] + '\\' + aim_array[j]).c_str()))
                cnt++;
            else
                break;
            if(cnt == aim_num)
                return i;
        }
    }
    return -1;
}

bool MainWindow::install(std::string name){
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", env.value("Path") + (";" + vs_path + "\\bin").c_str());
    process.setProcessEnvironment(env);
    process.start("cmd.exe", QStringList() << "/c" << "code --list-extensions");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    if (output.contains(name.c_str())){
        qDebug() << "have this extension";
        return true;
    }
    ui->notice_1->setText("正在为VS Code安装插件，请稍后");
    ui->notice_1->repaint();
    process.start("cmd.exe", QStringList() << "/c" << ("code --install-extension " + name).c_str());
    if (!process.waitForFinished(15000)) {
        process.terminate(); // 中止进程
        if(!process.waitForFinished(1500)){
            process.kill();
        }
        return false;
    }
    return true;
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray username = qgetenv("USERNAME");
    std::string path_arr[3];
    path_arr[0] = "C:\\users\\" + QString::fromLocal8Bit(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code";
    path_arr[1] = "D:\\Microsoft VS Code"; //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    path_arr[2] = vs_path;
    std::string aim_arr[1] = {"Code.exe"};
    int num = path_check(aim_arr, 1, path_arr, 3);
    if (num != -1) {
        // 找到
        ui->cnext_button_1->show();
        vs_path = path_arr[num];
        qDebug() << vs_path;
        ui->notice_1->setText(QString("成功找到VS Code"));
        ui->stackedWidget->setCurrentIndex(1);
        return;
    } else {
        //没找到
        ui->stackedWidget->setCurrentIndex(1);
        ui->notice_1->setText(QString("没能自动找到VS Code"));
        ui->cnext_button_1->hide();
        return;
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
    std::string path_arr[3];
    path_arr[0] = "C:\\users\\" + QString(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code";
    path_arr[1] = "D:\\Microsoft VS Code"; //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    path_arr[2] = ui->path_input->text().toStdString();
    std::string aim_arr[1] = {"Code.exe"};
    int num = path_check(aim_arr, 1, path_arr, 3);
    if (num != -1) {
        // 找到
        ui->cnext_button_1->show();
        vs_path = path_arr[num];
        ui->notice_1->setText(QString("成功找到VS Code"));
        return;
    } else {
        //没找到
        ui->notice_1->setText(QString("没能找到VS Code"));
        return;
    }
}


void MainWindow::on_cnext_button_1_clicked()
{
    if(!install("ms-ceintl.vscode-language-pack-zh-hans")){
        ui->notice_1->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    if(!install("ms-vscode.cpptools")){
        ui->notice_1->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    if(!install("guyutongxue.pause-console")){
        ui->notice_1->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    ui->notice_2->setText(" ");
    ui->cnext_button_2->hide();
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_11_clicked()
{
    QUrl url("https://github.com/brechtsanders/winlibs_mingw/releases/download/14.1.0posix-18.1.5-11.0.1-ucrt-r1/winlibs-x86_64-posix-seh-gcc-14.1.0-mingw-w64ucrt-11.0.1-r1.7z");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_13_clicked()
{
    std::string path_arr[3];
    path_arr[0] = ui->path_input_2->text().toStdString();
    path_arr[1] = path_arr[0] + "\\bin";
    path_arr[2] = path_arr[0] + "\\mingw64\\bin";
    std::string aim_arr[2];
    aim_arr[0] = "g++.exe";
    aim_arr[1] = "gdb.exe";
    int num = path_check(aim_arr, 2, path_arr, 3);
    if (num != -1) {
        // 找到
        ui->cnext_button_2->show();
        gcc_path = path_arr[num];
        ui->notice_2->setText(QString("成功找到mingw编译器"));
        return;
    } else {
        //没找到
        ui->notice_2->setText(QString("没能找到mingw编译器"));
        return;
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    QUrl url("https://7-zip.org/a/7z2406-x64.exe");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_18_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/Microsoft VS Code",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input->setText(dir);
}


void MainWindow::on_pushButton_19_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/bin",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input_2->setText(dir);
}


void MainWindow::on_pushButton_7_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input_3->setText(dir);
}


void MainWindow::on_cnext_button_2_clicked()
{
    ui->notice_3->setText(" ");
    ui->cnext_button_3->hide();
    ui->cnext_button_4->hide();
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_5_clicked()
{
    QUrl url("https://www.lanzouh.com/iGwmo201g7cj");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_16_clicked()
{
    Stringoperator s;
    if(s.check_asc(ui->path_input_3->text().toStdString())){
        QDir dir;
        if (!dir.exists(ui->path_input_3->text())) {
            if(dir.mkpath(ui->path_input_3->text())){
                ui->notice_3->setText("检查已通过，文件夹创建成功");
                ui->cnext_button_3->show();
                ui->cnext_button_4->show();
                work_dir = ui->path_input_3->text().toStdString();
            }
            else{
                ui->notice_3->setText("无法按照对应路径创建文件夹，请检查路径合理性");
            }
        }
        else{
            ui->notice_3->setText("检查已通过，文件夹已存在");
            ui->cnext_button_3->show();
            ui->cnext_button_4->show();
            work_dir = ui->path_input_3->text().toStdString();
        }
    }
    else{
        ui->notice_3->setText("路径中含有非法字符，文件夹创建失败");
    }
}


void MainWindow::on_cnext_button_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->cnext_button_5->hide();
    ui->notice_4->setText("正在按照推荐配置自动生成配置文件\n请耐心等待");
    ui->notice_4->repaint();
    Json_operator js;
    bool flag = true;
    flag = flag & js.write_launch(work_dir, gcc_path);
    flag = flag & js.write_properties(work_dir, gcc_path);
    flag = flag & js.write_tasks(work_dir, gcc_path);
    flag = flag & js.create_key_bind("F6", "run and pause", "workbench.action.tasks.runTask", "cpp");
    if(!flag){
        ui->notice_4->setText("很抱歉\n程序生成配置文件时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
        ui->cnext_button_5->show();
        return;
    }
    else{
        QString sourceFile = ":/jsondoc/jsondoc/hello_world.cpp";
        QString targetFile = (work_dir + "\\hello_world.cpp").c_str();
        if(QFile::copy(sourceFile, targetFile)){
            QProcess process;
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", env.value("Path") + (";" + vs_path + "\\bin").c_str());
            process.setProcessEnvironment(env);
            process.start("cmd.exe", QStringList() << "/c" << ("cd /d " + work_dir + "&& code . && code " + "hello_world.cpp").c_str());
            if(!process.waitForFinished(10000)){
                ui->notice_4->setText("很抱歉\n程序启动VS Code时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
                ui->cnext_button_5->show();
                return;
            }
            ui->notice_4->setText("程序已经完成自动配置\n感谢您使用VS Code Helper");
            ui->cnext_button_5->show();
            return;
        }
        else{
            ui->notice_4->setText("很抱歉\n程序生成测试源码时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
            ui->cnext_button_5->show();
            return;
        }
    }
}


void MainWindow::on_cnext_button_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_cnext_button_6_clicked()
{
    bool show_launch = !ui->checkBox_launch1->isChecked();
    bool show_test = !ui->checkBox_test1->isChecked();
    bool stop = ui->checkBox_launch2->isChecked();
    bool warn_err = ui->checkBox_test2->isChecked();
    std::string cpp_std = ui->comboBox_cppstd->currentText().toStdString();
    int optimize = ui->comboBox_optimize->currentIndex();
    int warn = ui->comboBox_warn->currentIndex();
    //copy from 272~309
    ui->stackedWidget->setCurrentIndex(4);
    ui->cnext_button_5->hide();
    ui->notice_4->setText("正在按照自定义配置自动生成配置文件\n请耐心等待");
    ui->notice_4->repaint();
    Json_operator js;
    bool flag = true;
    flag = flag & js.write_launch(work_dir, gcc_path, show_launch, stop);
    flag = flag & js.write_properties(work_dir, gcc_path, cpp_std);
    flag = flag & js.write_tasks(work_dir, gcc_path, cpp_std, show_test, optimize, warn, warn_err);
    flag = flag & js.create_key_bind("F6", "run and pause", "workbench.action.tasks.runTask", "cpp");
    if(!flag){
        ui->notice_4->setText("很抱歉\n程序生成配置文件时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
        ui->cnext_button_5->show();
        return;
    }
    else{
        QString sourceFile = ":/jsondoc/jsondoc/hello_world.cpp";
        QString targetFile = (work_dir + "\\hello_world.cpp").c_str();
        if(QFile::copy(sourceFile, targetFile)){
            QProcess process;
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", env.value("Path") + (";" + vs_path + "\\bin").c_str());
            process.setProcessEnvironment(env);
            process.start("cmd.exe", QStringList() << "/c" << ("cd /d " + work_dir + "&& code . && code " + "hello_world.cpp").c_str());
            if(!process.waitForFinished(10000)){
                ui->notice_4->setText("很抱歉\n程序启动VS Code时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
                ui->cnext_button_5->show();
                return;
            }
            ui->notice_4->setText("程序已经完成自动配置\n感谢您使用VS Code Helper");
            ui->cnext_button_5->show();
            return;
        }
        else{
            ui->notice_4->setText("很抱歉\n程序生成测试源码时出现问题\n请返回重试或将问题提交至github界面\n感谢您的支持");
            ui->cnext_button_5->show();
            return;
        }
    }
}


void MainWindow::on_cnext_button_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

