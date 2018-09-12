#include "facerecognizer.h"
#include <iostream>

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
        extract_image_chip(imgdlib, get_face_chip_details(shape, 150, 0.25),
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

void Recognizer::printFaceDescriptions(std::vector<matrix<float,0,1>> face_descriptors)
{
    int count = 0;
    for(auto description : face_descriptors)
    {
        cout << "Face "<< count <<": " << trans(description) <<endl;
        count++;
    }
}
