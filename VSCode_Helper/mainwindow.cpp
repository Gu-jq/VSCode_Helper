#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QFileDialog>
#include "string_operator.h"
#include <QDir>
#include "json_operator.h"
#include <vector>
#include <QSettings>
#include <QFileInfo>
#include "path_operator.h"
//windeployqt: 为exe链接dll的指令

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


bool path_check(std::string aim_array[], int aim_num, std::vector<std::string>& path_array, std::string &path){
    for(std::string& s : path_array){
        int cnt = 0;
        for (int j = 0; j < aim_num; j++){
            qDebug() << s;
            if(QFile::exists((s + '\\' + aim_array[j]).c_str()))
                cnt++;
            else
                break;
            if(cnt == aim_num){
                path = s;
                return true;
            }
        }
    }
    return false;
}


void get_PATH(std::vector<std::string>* path_array){
    QSettings reg("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", QSettings::NativeFormat);
    QString path = reg.value("Path").toString();
    QStringList paths = path.split(";");
    for(QString &s : paths){
        path_array->push_back(s.toStdString());
    }
    QSettings reg2("HKEY_CURRENT_USER\\Environment", QSettings::NativeFormat);
    path = reg2.value("Path").toString();
    paths = path.split(";");
    for(QString &s : paths){
        path_array->push_back(s.toStdString());
    }
}


void set_PATH(std::string path){
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << ("setx PATH \"%PATH%;" + path + "\"").c_str());
    process.waitForFinished();
}

//can't use when testing
void find_where(std::vector<std::string>* path_array, std::string cmd){
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << ("where " + cmd).c_str());
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList list = output.split('\n');
    for(QString& s : list){
        QFileInfo fileInfo(s);
        QString dir = fileInfo.absolutePath();
        path_array->push_back(dir.toStdString());
    }
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
    std::vector<std::string> path_arr;
    path_arr.push_back(vs_path);
    path_arr.push_back("C:\\users\\" + QString::fromLocal8Bit(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code");
    path_arr.push_back("D:\\Microsoft VS Code"); //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    std::string aim_arr[1] = {"Code.exe"};
    get_PATH(&path_arr);
    if (path_check(aim_arr, 1, path_arr, vs_path)) {
        // 找到
        ui->cnext_button_1->show();
        ui->path_input->setText(vs_path.c_str());
        ui->notice_1->setText(QString("已自动找到VS Code"));
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
    std::vector<std::string> path_arr;
    path_arr.push_back(ui->path_input->text().toStdString());
    path_arr.push_back("C:\\users\\" + QString::fromLocal8Bit(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code");
    path_arr.push_back("D:\\Microsoft VS Code"); //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    std::string aim_arr[1] = {"Code.exe"};
    get_PATH(&path_arr);
    if (path_check(aim_arr, 1, path_arr, vs_path)) {
        // 找到
        ui->cnext_button_1->show();
        ui->notice_1->setText(QString("成功找到VS Code"));
        return;
    } else {
        //没找到
        ui->notice_1->setText(QString("没能找到VS Code"));
        ui->cnext_button_1->hide();
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

    std::vector<std::string> path_arr;
    path_arr.push_back(gcc_path);
    get_PATH(&path_arr);
    std::string aim_arr[2];
    aim_arr[0] = "g++.exe";
    aim_arr[1] = "gdb.exe";
    if (path_check(aim_arr, 2, path_arr, gcc_path)) {
        // 找到
        ui->cnext_button_2->show();
        ui->path_input_2->setText(gcc_path.c_str());
        ui->notice_2->setText(QString("已自动找到mingw编译器"));
    } else {
        //没找到
        ui->notice_2->setText(" ");
        ui->cnext_button_2->hide();
        ui->notice_2->setText(QString("没能自动找到mingw编译器"));
    }
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
    std::vector<std::string> path_arr;
    path_arr.push_back(ui->path_input_2->text().toStdString());
    path_arr.push_back(path_arr[0] + "\\bin");
    path_arr.push_back(path_arr[0] + "\\mingw64\\bin");
    get_PATH(&path_arr);
    std::string aim_arr[2];
    aim_arr[0] = "g++.exe";
    aim_arr[1] = "gdb.exe";
    if (path_check(aim_arr, 2, path_arr, gcc_path)) {
        // 找到
        ui->cnext_button_2->show();
        ui->notice_2->setText(QString("成功找到mingw编译器"));
        return;
    } else {
        //没找到
        ui->cnext_button_2->hide();
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
    Stringoperator op;
    QProcess process;
    process.start((gcc_path + "\\g++.exe").c_str() , QStringList() << "--version");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    int ver = op.get_version(output.toStdString());
    qDebug() << output.toStdString();
    if(ver < 80000){
        ui->notice_2->setText(QString("检测到mingw编译器版本过低，您应当下载新版"));
        return;
    }
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


void MainWindow::on_pushButton_2_clicked()
{

    QByteArray username = qgetenv("USERNAME");
    std::vector<std::string> path_arr;
    path_arr.push_back(vs_path);
    path_arr.push_back("C:\\users\\" + QString::fromLocal8Bit(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code");
    path_arr.push_back("D:\\Microsoft VS Code"); //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    std::string aim_arr[1] = {"Code.exe"};
    get_PATH(&path_arr);
    if (path_check(aim_arr, 1, path_arr, vs_path)) {
        // 找到
        ui->pynext_button_1->show();
        ui->path_input_4->setText(vs_path.c_str());
        ui->notice_6->setText(QString("已自动找到VS Code"));
        ui->stackedWidget->setCurrentIndex(6);
        return;
    } else {
        //没找到
        ui->stackedWidget->setCurrentIndex(6);
        ui->notice_6->setText(QString("没能自动找到VS Code"));
        ui->pynext_button_1->hide();
        return;
    }
}


void MainWindow::on_pushButton_23_clicked()
{
    QUrl url("https://code.visualstudio.com/Download");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}



void MainWindow::on_pushButton_24_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/Microsoft VS Code",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input_4->setText(dir);
}



void MainWindow::on_pynext_button_1_clicked()
{
    if(!install("ms-ceintl.vscode-language-pack-zh-hans")){
        ui->notice_6->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    if(!install("ms-python.python")){
        ui->notice_6->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    if(!install("ms-python.debugpy")){
        ui->notice_6->setText(QString("插件安装失败，请检查网络连接"));
        return;
    }
    ui->stackedWidget->setCurrentIndex(7);
}


void MainWindow::on_pushButton_25_clicked()
{
    QByteArray username = qgetenv("USERNAME");
    std::vector<std::string> path_arr;
    path_arr.push_back(ui->path_input_4->text().toStdString());
    path_arr.push_back("C:\\users\\" + QString::fromLocal8Bit(username).toStdString() + "\\AppData\\Local\\Programs\\Microsoft VS Code");
    path_arr.push_back("D:\\Microsoft VS Code"); //测试阶段，这里改成X盘使得VSCode无法直接被找到，记得改回去
    std::string aim_arr[1] = {"Code.exe"};
    get_PATH(&path_arr);
    if (path_check(aim_arr, 1, path_arr, vs_path)) {
        // 找到
        ui->pynext_button_1->show();
        ui->notice_6->setText(QString("成功找到VS Code"));
        return;
    } else {
        //没找到
        ui->pynext_button_1->hide();
        ui->notice_6->setText(QString("没能找到VS Code"));
        return;
    }
}


void MainWindow::on_pynext_button_2_1_clicked()
{
    std::vector<std::string> path_arr;
    path_arr.push_back(py_path);
    get_PATH(&path_arr);
    std::string aim_arr[1] = {"python.exe"};
    if (path_check(aim_arr, 1, path_arr, py_path)) {
        // 找到
        ui->pynext_button_3_1->show();
        ui->path_input_5->setText(py_path.c_str());
        ui->notice_7->setText(QString("已自动找到Python"));
        ui->stackedWidget->setCurrentIndex(8);
        return;
    } else {
        //没找到
        ui->stackedWidget->setCurrentIndex(8);
        ui->notice_6->setText(QString("没能自动找到Python"));
        ui->pynext_button_3_1->hide();
        return;
    }
}


void MainWindow::on_pushButton_26_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_8_clicked()
{
    QUrl url("https://www.python.org/downloads/");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_27_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), " ",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input_5->setText(dir);
}


void MainWindow::on_pushButton_29_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_17_clicked()
{
    std::vector<std::string> path_arr;
    path_arr.push_back(py_path);
    get_PATH(&path_arr);
    std::string aim_arr[1] = {"python.exe"};
    if (path_check(aim_arr, 1, path_arr, py_path)) {
        // 找到
        ui->pynext_button_3_1->show();
        ui->notice_7->setText(QString("成功找到Python"));
        return;
    } else {
        //没找到
        ui->notice_6->setText(QString("没能找到Python"));
        ui->pynext_button_3_1->hide();
        return;
    }
}


void MainWindow::on_pynext_button_3_1_clicked()
{
    std::vector<std::string> path_arr;
    get_PATH(&path_arr);
    if(py_path[py_path.size() - 1] == '\\' && py_path[py_path.size() - 1] == '/')
        py_path.pop_back();
    bool flag1 = true, flag2 = true;
    QString path1 = QDir::cleanPath(py_path.c_str());
    QString path2 = QDir::cleanPath((py_path + "/Scripts").c_str());
    for(std::string& s : path_arr){
        QString path = QDir::cleanPath(s.c_str());
        if(path1 == path)
            flag1 = false;
        if(path2 == path)
            flag2 = false;
    }
    if(flag1){
        qDebug() << "Don't have python in PATH";
        set_PATH(path1.toStdString());
    }
    if(flag2){
        qDebug() << "Don't have python/Scripts in PATH";
        set_PATH(path2.toStdString());
    }
    ui->stackedWidget->setCurrentIndex(9);
    ui->url_input_1->setText("https://pypi.tuna.tsinghua.edu.cn/simple");
    ui->key_input_1->setText("F6");
}


void MainWindow::on_pushButton_21_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", env.value("Path") + (";" + vs_path + "\\bin").c_str());
    process.setProcessEnvironment(env);
    process.start("cmd.exe", QStringList() << "/c" << "code");
    process.waitForFinished();
}


void MainWindow::on_pushButton_22_clicked()
{
    ui->url_input_1->setText("https://pypi.org/simple/");
}


void MainWindow::on_pushButton_20_clicked()
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "python -m pip install --upgrade pip");
    process.waitForFinished();
    process.start("cmd.exe", QStringList() << "/c" << ("pip pip config set global.index-url " + ui->url_input_1->text()));
    process.waitForFinished();
}


void MainWindow::on_pushButton_30_clicked()
{
    if(install("ms-python.debugpy")){
        ui->pushButton_30->setText("Pylance安装成功！");
    }
}


void MainWindow::on_pynext_button_2_2_clicked()
{
    std::vector<std::string> path_arr;
    path_arr.push_back(conda_path);
    get_PATH(&path_arr);
    std::string aim_arr[1];
    aim_arr[0] = "conda.exe";
    if (path_check(aim_arr, 1, path_arr, conda_path)) {
        // 找到
        ui->pynext_button_3_2->show();
        ui->path_input_6->setText(conda_path.c_str());
        ui->notice_8->setText(QString("成功找到conda"));
    } else {
        //没找到
        ui->notice_8->setText(" ");
        ui->pynext_button_3_2->hide();
        ui->notice_8->setText(QString("没能找到conda"));
    }
    ui->stackedWidget->setCurrentIndex(10);
    return;

}


void MainWindow::on_pushButton_31_clicked()
{
    QUrl url("https://repo.anaconda.com/miniconda/Miniconda3-latest-Windows-x86_64.exe");
    if (!QDesktopServices::openUrl(url)) {
        return;
    }
}


void MainWindow::on_pushButton_32_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/conda",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->path_input_6->setText(dir);
}


void MainWindow::on_pushButton_34_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pynext_button_3_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(11);
    std::vector<std::string> name, version, path;
    Path_operator p;
    p.get_version(&name, &path, &version, conda_path);
    ui->listWidget->clear();
    for(int i = 0; i < name.size(); i++){
        std::string now = name[i] + "  (" + version[i] + ')';
        ui->listWidget->addItem(now.c_str());
    }
}


void MainWindow::on_pushButton_33_clicked()
{
    std::vector<std::string> path_arr;
    path_arr.push_back(ui->path_input_6->text().toStdString());
    get_PATH(&path_arr);
    std::string aim_arr[1];
    aim_arr[0] = "conda.exe";
    if (path_check(aim_arr, 1, path_arr, conda_path)) {
        // 找到
        ui->pynext_button_3_2->show();
        ui->notice_8->setText(QString("成功找到conda"));
        return;
    } else {
        //没找到
        ui->pynext_button_3_2->hide();
        ui->notice_8->setText(QString("没能找到conda"));
        return;
    }
}


void MainWindow::on_pushButton_36_clicked()
{
    std::vector<std::string> name, version, path;
    Path_operator p;
    p.get_version(&name, &path, &version, conda_path);
    ui->listWidget->clear();
    for(int i = 0; i < name.size(); i++){
        std::string now = name[i] + "  (" + version[i] + ')';
        ui->listWidget->addItem(now.c_str());
    }
}


void MainWindow::on_pushButton_35_clicked()
{
    if(ui->ver_input->text().size() <= 1 || ui->name_input->text().size() == 0)
        return;
    QProcess process;
    ui->pushButton_35->setText("正在创建，请耐心等待");
    process.start((conda_path + "\\conda.exe").c_str(), QStringList() << "create" << "--name" << ui->name_input->text() << "python=" + ui->ver_input->text());
    process.waitForFinished(300000);
    std::vector<std::string> name, version, path;
    Path_operator p;
    p.get_version(&name, &path, &version, conda_path);
    ui->listWidget->clear();
    for(int i = 0; i < name.size(); i++){
        std::string now = name[i] + "  (" + version[i] + ')';
        ui->listWidget->addItem(now.c_str());
    }
    ui->pushButton_35->setText("创建新python环境");
}


void MainWindow::on_pushButton_37_clicked()
{
    if(install("ms-python.debugpy")){
        ui->pushButton_37->setText("Pylance安装成功！");
    }
}


void MainWindow::on_pushButton_39_clicked()
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "python -m pip install --upgrade pip");
    process.waitForFinished();
    process.start("cmd.exe", QStringList() << "/c" << ("pip pip config set global.index-url " + ui->url_input_2->text()));
    process.waitForFinished();
}


void MainWindow::on_pushButton_40_clicked()
{
    ui->url_input_2->setText("https://pypi.org/simple/");
}


void MainWindow::on_pushButton_41_clicked()
{

}


void MainWindow::on_pushButton_28_clicked()
{

}


void MainWindow::on_pushButton_42_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", env.value("Path") + (";" + vs_path + "\\bin").c_str());
    process.setProcessEnvironment(env);
    process.start("cmd.exe", QStringList() << "/c" << "code");
    process.waitForFinished();
}


void MainWindow::on_pushButton_38_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pynext_button_4_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
    ui->url_input_2->setText("https://pypi.tuna.tsinghua.edu.cn/simple");
    ui->key_input_2->setText("F6");

}


void MainWindow::on_pushButton_43_clicked()
{
    if(install("donjayamanne.python-environment-manager")){
        ui->pushButton_37->setText("Environment Manger安装成功！");
    }
}

