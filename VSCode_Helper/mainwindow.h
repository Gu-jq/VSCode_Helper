#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_cnext_button_1_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_7_clicked();

    void on_cnext_button_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_16_clicked();

    void on_cnext_button_3_clicked();

    void on_cnext_button_5_clicked();

    void on_pushButton_15_clicked();

    void on_cnext_button_6_clicked();

    void on_cnext_button_4_clicked();

private:
    bool install(std::string name);
    Ui::MainWindow *ui;
    QProcess *process_code;
    std::string vs_path, gcc_path, work_dir;
    bool find_vsc;
};
#endif // MAINWINDOW_H
