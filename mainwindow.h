#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include "facerecognizer.h"
#include "datamanager.h"
#include <map>
#include <mutex>

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
    void showFrame(std::mutex& fr_mutex, cv::Mat& frameThread, cv::VideoCapture& cap);
    void initPara(std::string cfg_fn);

private slots:
    void on_pushOpenCamera_clicked();

    void on_pushCloseCamera_clicked();

    void on_pushCloseApp_clicked();

    void on_pushRegistration_clicked();

    void on_pushCheckIn_clicked();

public:
    cv::Mat frame;

private:
    Ui::MainWindow *ui;

    cv::VideoCapture video;
    bool isOpenCamera;
    bool isInProcess;

    std::string detector_model;
    std::string shape_predictor_model;
    std::string face_recognition_model;
    std::string database_path;
    std::string proto_ssd_file;
    std::string ssd_model;
    std::map<std::string, int> name_counter;

public:
    bool isIPCam;
    int camID;
    bool isRecognizing;
    float margin_dlib;
    std::string camURL;
};

#endif // MAINWINDOW_H
