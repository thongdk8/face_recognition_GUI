#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "facerecognizer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);
    isInProcess = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFrame()
{
    cv::Mat image;
    std::string detector_model = "/home/thongpb/works/face_recognition/dlib-models-master/mmod_human_face_detector.dat";
    std::string shape_predictor_model = "/home/thongpb/works/face_recognition/dlib-models-master/shape_predictor_68_face_landmarks.dat";
    std::string face_recognition_model = "/home/thongpb/works/face_recognition/dlib-models-master/dlib_face_recognition_resnet_model_v1.dat";

    Recognizer face_recognizer(detector_model, shape_predictor_model, face_recognition_model);

    while(isInProcess)
    {
        if(video.isOpened() && isOpenCamera)
        {
            video.read(image);
            cv::resize(image, image, cv::Size(1280,720));

            auto dess = face_recognizer.computeFaceDescriptors(image);
            face_recognizer.printFaceDescriptions(dess);

            image = face_recognizer.getImage();

            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
            ui->imageScreen->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols,
                                                                 image.rows, image.step,
                                                                 QImage::Format_RGB888)));
        }
        QApplication::processEvents();
        if(!isInProcess) break;
    }
    video.release();
}

void MainWindow::on_pushOpenCamera_clicked()
{
    std::cout << "on open camera push bottom" << std::endl;
    video.open(0);
    isOpenCamera = true;
}

void MainWindow::on_pushCloseCamera_clicked()
{
    isOpenCamera = false;
}

void MainWindow::on_pushCloseApp_clicked()
{
    isInProcess = false;
    QApplication::exit(0);
}
