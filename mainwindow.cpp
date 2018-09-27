#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    isInProcess = true;
    isIPCam = false;
    camID = 0;
    camURL = "";

    initPara("camconfig.cfg");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initPara(std::string cfg_fn) {
    ifstream cfg_file;
    cfg_file.open(cfg_fn);
    cfg_file >> isIPCam;
    cfg_file >> camID;
    cfg_file >> camURL;
    cfg_file >> detector_model;
    cfg_file >> shape_predictor_model;
    cfg_file >> face_recognition_model;
    cfg_file >> database_path;
    cfg_file >> margin_dlib;
    cfg_file >> proto_ssd_file;
    cfg_file >> ssd_model;
    cfg_file.close();

    isRecognizing = false;
    isOpenCamera = true;
}

void MainWindow::showFrame() {
    cv::Mat image;

    initPara("camconfig.cfg");
    Recognizer face_recognizer(detector_model, shape_predictor_model,
                               face_recognition_model);

    //    SSDFaceDetector face_detector(proto_ssd_file,ssd_model,0.2,true);

    DataManager dm(database_path + "para.txt");
    dm.loadData(database_path + "data.txt", database_path + "name_data.txt");
    dm.initSearchEngine();

    video.set(CAP_PROP_BUFFERSIZE, 3);

    VideoWriter video1("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 5,
                       Size(960, 540));
    VideoWriter video2("outcpp1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 5,
                       Size(960, 540));

    while (isInProcess) {
        if (video.isOpened() && isOpenCamera) {
            video.read(image);

            if (!image.empty()) {
                cv::resize(image, image, cv::Size(960, 540));
                video1.write(image);
                if (isRecognizing) {
                    std::vector<cv::Point> anchors;
                    auto dess = face_recognizer.computeFaceDescriptors(
                        image, anchors, margin_dlib);
                    //                    auto dess =
                    //                    face_recognizer.computeFaceDescriptors(face_detector,image,anchors,
                    //                    margin_dlib);

                    //                face_recognizer.printFaceDescriptions(dess);
                    auto res_names = dm.getInstanceName(dess);
                    image = face_recognizer.getImage();

                    if (res_names.size() > 0) {
                        for (int i = 0; i < res_names.size(); i++) {
                            cv::putText(image, res_names[i].c_str(), anchors[i],
                                        cv::FONT_HERSHEY_PLAIN, 1.0,
                                        cv::Scalar(0, 0, 255), 2);
                            std::cout << "Face #" << i
                                      << " in the frame is detected"
                                      << std::endl;
                            name_counter[res_names[i]]++;
                        }
                    }
                }
                video2.write(image);
                cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
                ui->imageScreen->setPixmap(QPixmap::fromImage(
                    QImage(image.data, image.cols, image.rows, image.step,
                           QImage::Format_RGB888)));
            }
        }
        QApplication::processEvents();
        if (!isInProcess)
            break;
    }
    video.release();
    video1.release();
    video2.release();
}

void MainWindow::showFrame(std::mutex &fr_mutex, cv::Mat &frameThread,
                           cv::VideoCapture &cap) {
    cv::Mat image;
    Recognizer face_recognizer(detector_model, shape_predictor_model,
                               face_recognition_model);

    //    SSDFaceDetector face_detector(proto_ssd_file,ssd_model,0.2,true);

    DataManager dm(database_path + "para.txt");
    dm.loadData(database_path + "data.txt", database_path + "name_data.txt");
    dm.initSearchEngine();

    //    video.set(CAP_PROP_BUFFERSIZE, 3);

    VideoWriter video1("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 5,
                       Size(960, 540));
    VideoWriter video2("outcpp1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 5,
                       Size(960, 540));

    while (isInProcess) {
        fr_mutex.lock();
        image = frameThread.clone();
        fr_mutex.unlock();

        //            video.read(image);

        if (!image.empty() && isOpenCamera) {
            cv::resize(image, image, cv::Size(960, 540));

            if (isRecognizing) {
                video1.write(image);
                std::vector<cv::Point> anchors;
                auto dess = face_recognizer.computeFaceDescriptors(
                    image, anchors, margin_dlib);
                //                    auto dess =
                //                    face_recognizer.computeFaceDescriptors(face_detector,image,anchors,
                //                    margin_dlib);

                //                face_recognizer.printFaceDescriptions(dess);
                auto res_names = dm.getInstanceName(dess);
                image = face_recognizer.getImage();

                if (res_names.size() > 0) {
                    for (int i = 0; i < res_names.size(); i++) {
                        cv::putText(image, res_names[i].c_str(), anchors[i],
                                    cv::FONT_HERSHEY_PLAIN, 1.0,
                                    cv::Scalar(0, 0, 255), 2);
                        std::cout << "Face #" << i
                                  << " in the frame is detected" << std::endl;
                        name_counter[res_names[i]]++;
                    }
                }
                video2.write(image);
            }

            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
            ui->imageScreen->setPixmap(
                QPixmap::fromImage(QImage(image.data, image.cols, image.rows,
                                          image.step, QImage::Format_RGB888)));
        }
        QApplication::processEvents();
        if (!isInProcess)
            break;
    }
    cap.release();
    //    video.release();
    video1.release();
    video2.release();
}

void MainWindow::on_pushOpenCamera_clicked() {
    std::cout << "on open camera push bottom" << std::endl;

//    if (isIPCam)
//        video.open(camURL);
//    else
//        video.open(camID);

    isOpenCamera = true;
}

void MainWindow::on_pushCloseCamera_clicked() { isOpenCamera = false; }

void MainWindow::on_pushCloseApp_clicked() {
    isInProcess = false;
    ofstream log_file;
    log_file.open("log.txt");
    for (auto c : name_counter) {
        log_file << c.first << ", " << c.second << std::endl;
    }
    log_file.close();

    QApplication::quit();
}

void MainWindow::on_pushRegistration_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Register");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    form.addRow(new QLabel("Fill the form!"));

    // Add the lineEdits with their respective labels
    QList<QLineEdit *> fields;

    //    for(int i = 0; i < 4; ++i) {
    //        QLineEdit *lineEdit = new QLineEdit(&dialog);
    //        QString label = QString("Value %1").arg(i + 1);
    //        form.addRow(label, lineEdit);

    //        fields << lineEdit;
    //    }

    QLineEdit *lineEdit = new QLineEdit(&dialog);
    QString label = QString("Name");
    form.addRow(label, lineEdit);
    fields << lineEdit;

    QLineEdit *lineEdit1 = new QLineEdit(&dialog);
    QString label1 = QString("MSSV");
    form.addRow(label1, lineEdit1);
    fields << lineEdit1;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        foreach (QLineEdit *lineEdit, fields) {
            std::cout << lineEdit->text().toStdString() << std::endl;
        }
    }
}

void MainWindow::on_pushCheckIn_clicked() { isRecognizing = !isRecognizing; }
