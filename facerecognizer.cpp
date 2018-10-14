#include "facerecognizer.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>

//#include <dlib/opencv.h>
//#include <dlib/opencv/to_open_cv.h>
//#include <dlib/opencv/cv_image.h>
Recognizer::Recognizer(string detector_model, string shape_predictor_model, string recognition_model)
{
    deserialize(detector_model) >> detector;
    deserialize(shape_predictor_model) >> sp;
    deserialize(recognition_model) >> net;
    std::cout << "Load trained models sucessfully" << std::endl;
}
void Recognizer::setImage(Mat &image)
{
    this->image = image;
}

Mat Recognizer::getImage()
{
    return this->image;
}

std::vector<matrix<float,0,1>> Recognizer::computeFaceDescriptors(Mat &img)
{
    setImage(img);
    std::vector<matrix<float,0,1>> face_descriptors;

    dlib::cv_image<rgb_pixel> imgcvdlib(image);
    dlib::matrix<rgb_pixel> imgdlib;
    dlib::assign_image(imgdlib, imgcvdlib);

    std::vector<matrix<rgb_pixel>> faces;
    auto detectedFaces = detector(imgdlib);
    for (auto face : detectedFaces)
    {
        auto shape = sp(imgdlib, face.rect);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgdlib, get_face_chip_details(shape, 150, 0.5),
                           face_chip);
        faces.push_back(face_chip);
        //        cv::Mat face_chip_cv = dlib::toMat(face_chip);
        //        cv::imshow("face", face_chip_cv);
        //        cv::imwrite(argv[2]+std::to_string(count_face) + ".png", face_chip_cv);
        long left_cor = face.rect.left();
        long right_cor = face.rect.right();
        long top_cor = face.rect.top();
        long bottom_cor = face.rect.bottom();
        cv::rectangle(image, cv::Point(left_cor, top_cor),
                      cv::Point(right_cor, bottom_cor),
                      cv::Scalar(255, 120, 120), 2);
    }
    if (faces.size() == 0)
    {
        cout << "No faces found in image!" << endl;
    }
    else
    {
        face_descriptors = net(faces);
    }

    return face_descriptors;
}

std::vector<matrix<float,0,1>> Recognizer::computeFaceDescriptors(Mat &img, std::vector<cv::Point> &anchors, float margin_dlib)
{
    setImage(img);
    std::vector<matrix<float,0,1>> face_descriptors;

    dlib::cv_image<rgb_pixel> imgcvdlib(image);
    dlib::matrix<rgb_pixel> imgdlib;
    dlib::assign_image(imgdlib, imgcvdlib);

    std::vector<matrix<rgb_pixel>> faces;
    auto detectedFaces = detector(imgdlib);
    for (auto face : detectedFaces)
    {
        auto shape = sp(imgdlib, face.rect);
        matrix<rgb_pixel> face_chip;

//        for(float margin = margin_dlib; margin < 0.46; margin = margin + 0.05)
//        {
            extract_image_chip(imgdlib, get_face_chip_details(shape, 150, margin_dlib),
                               face_chip);
            faces.push_back(face_chip);
//        }
//                cv::Mat face_chip_cv = dlib::toMat(face_chip);
        //        cv::imshow("face", face_chip_cv);
//                cv::imwrite(std::to_string(std::rand()%100) + ".png", face_chip_cv);
        long left_cor = face.rect.left();
        long right_cor = face.rect.right();
        long top_cor = face.rect.top();
        long bottom_cor = face.rect.bottom();
        cv::rectangle(image, cv::Point(left_cor, top_cor),
                      cv::Point(right_cor, bottom_cor),
                      cv::Scalar(255, 120, 120), 2);
        anchors.push_back (cv::Point(left_cor, top_cor));
    }
    if (faces.size() == 0)
    {
        cout << "No faces found in image!" << endl;
    }
    else
    {
        face_descriptors = net(faces);
    }

    return face_descriptors;
}

std::vector<matrix<float,0,1>> Recognizer::computeFaceDescriptors(Mat &img, std::vector<cv::Point> &anchors, float margin_dlib_base, int nb_crops)
{
    static cv::Mat img_tmp;
    static int cnt_face = 0;
    setImage(img);

    cv::cvtColor(image, img_tmp, cv::COLOR_BGR2RGB);
    std::vector<matrix<float,0,1>> face_descriptors;
    dlib::cv_image<rgb_pixel> imgcvdlib(img_tmp);
    dlib::matrix<rgb_pixel> imgdlib;
    dlib::assign_image(imgdlib, imgcvdlib);

    std::vector<matrix<rgb_pixel>> faces;
    auto detectedFaces = detector(imgdlib);
    for (auto face : detectedFaces)
    {
        auto shape = sp(imgdlib, face.rect);
        matrix<rgb_pixel> face_chip;

        float margin = margin_dlib_base;
        for(int j=0; j < nb_crops;j++)
        {
            extract_image_chip(imgdlib, get_face_chip_details(shape, 150, margin),
                               face_chip);
            if (margin < 0.34 && margin > 0.26)
            {
                dlib::save_png (face_chip, "/home/thongpb/Qt5.6.3/projects/build-face_recognition-Desktop_Qt_5_6_3_GCC_64bit-Release/face_patchs/" + std::to_string (cnt_face) + ".png");
                cnt_face++;
            }
            faces.push_back(face_chip);
            margin += 0.05;
        }

        long left_cor = face.rect.left();
        long right_cor = face.rect.right();
        long top_cor = face.rect.top();
        long bottom_cor = face.rect.bottom();
        cv::rectangle(image, cv::Point(left_cor, top_cor),
                      cv::Point(right_cor, bottom_cor),
                      cv::Scalar(255, 120, 120), 2);
        anchors.push_back (cv::Point(left_cor, top_cor));
    }
    if (faces.size() == 0)
    {
        cout << "No faces found in image!" << endl;
    }
    else
    {
        face_descriptors = net(faces);
    }

    return face_descriptors;
}

std::vector<matrix<float,0,1>> Recognizer::computeFaceDescriptors( SSDFaceDetector& face_detector,Mat &img, std::vector<cv::Point> &anchors, float margin_dlib)
{
    setImage(img);
    std::vector<matrix<float,0,1>> face_descriptors;

    dlib::cv_image<rgb_pixel> imgcvdlib(image);
    dlib::matrix<rgb_pixel> imgdlib;
    dlib::assign_image(imgdlib, imgcvdlib);

    std::vector<matrix<rgb_pixel>> faces;
    auto detectedFaces = face_detector.detectFrame (image, 2);
    for (auto face : detectedFaces)
    {
        auto shape = sp(imgdlib, face);
        matrix<rgb_pixel> face_chip;
        extract_image_chip(imgdlib, get_face_chip_details(shape, 150, margin_dlib),
                           face_chip);
        faces.push_back(face_chip);
//                cv::Mat face_chip_cv = dlib::toMat(face_chip);
        //        cv::imshow("face", face_chip_cv);
//                cv::imwrite(std::to_string(std::rand()%100) + ".png", face_chip_cv);
        long left_cor = face.left();
        long right_cor = face.right();
        long top_cor = face.top();
        long bottom_cor = face.bottom();
        cv::rectangle(image, cv::Point(left_cor, top_cor),
                      cv::Point(right_cor, bottom_cor),
                      cv::Scalar(255, 120, 120), 2);
        anchors.push_back (cv::Point(left_cor, top_cor));
    }
    if (faces.size() == 0)
    {
        cout << "No faces found in image!" << endl;
    }
    else
    {
        face_descriptors = net(faces);
    }

    return face_descriptors;
}

void Recognizer::printFaceDescriptions(std::vector<matrix<float,0,1>> face_descriptors)
{
    int count = 0;
    for(auto description : face_descriptors)
    {
        cout << "Face "<< count <<": " << trans(description) <<endl;
        count++;
    }
}
