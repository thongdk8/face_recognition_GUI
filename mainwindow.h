#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/videoio.hpp>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void showFrame();

private slots:
    void on_pushOpenCamera_clicked();

    void on_pushCloseCamera_clicked();

    void on_pushCloseApp_clicked();

private:
    Ui::MainWindow *ui;

    cv::VideoCapture video;
    bool isOpenCamera;
    bool isInProcess;
};

#endif // MAINWINDOW_H
