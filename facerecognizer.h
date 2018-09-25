#ifndef FACERECOGNIZER_H
#define FACERECOGNIZER_H
#include <dlib/opencv.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/string.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <vector>

#include "cnndetector.h"
#include "defineddnn.h"

using namespace dlib;
using namespace std;
using namespace cv;

class Recognizer
{
public:
    Recognizer(std::string detector_model, std::string shape_predictor_model, std::string recognition_model);

    void setImage(Mat &image);
    Mat getImage();

    std::vector<matrix<float,0,1>> computeFaceDescriptors(Mat &img);
    std::vector<matrix<float,0,1>> computeFaceDescriptors(Mat &img, std::vector<cv::Point> &anchors, float margin_dlib);
    std::vector<matrix<float,0,1>> computeFaceDescriptors( SSDFaceDetector& face_detector,Mat &img, std::vector<cv::Point> &anchors, float margin_dlib);
    void printFaceDescriptions(std::vector<matrix<float,0,1>> face_descriptors);

private:
    Mat image;
    net_type detector;
    shape_predictor sp;
    anet_type net;
};

#endif // FACERECOGNIZER_H
