#ifndef CNNDETECTOR_H
#define CNNDETECTOR_H
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <dlib/image_processing.h>

using namespace cv;
using namespace std;
using namespace cv::dnn;

const size_t inWidth = 300;
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const Scalar meanVal(104.0, 177.0, 123.0);

class SSDFaceDetector
{
public:
    SSDFaceDetector(std::string proto_file, std::string model_file, float threshold = 0.3, bool is_opencl = false)
    {
        //! [Initialize network]
        net = readNetFromCaffe(proto_file, model_file);
        //! [Initialize network]

        if (net.empty())
        {
            cerr << "Can't load network by" << endl;
        }
        if (is_opencl)
        {
            net.setPreferableTarget (DNN_TARGET_OPENCL);
        }
        confidenceThreshold = threshold;
    }

    void setThreshold(float thresholh)
    {
        confidenceThreshold = thresholh;
    }

    std::vector<dlib::rectangle> detectFrame(cv::Mat& frame, int offset)
    {
        std::vector<dlib::rectangle> dets;
        //! [Prepare blob]
        Mat inputBlob = blobFromImage(frame, inScaleFactor,
                                     Size(inWidth, inHeight), meanVal, false, false); //Convert Mat to batch of images
        //! [Prepare blob]

        //! [Set input blob]
        net.setInput(inputBlob, "data"); //set the network input
        //! [Set input blob]

        //! [Make forward pass]
        Mat detection = net.forward("detection_out"); //compute output
        //! [Make forward pass]

        Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
        for(int i = 0; i < detectionMat.rows; i++)
                {
                    float confidence = detectionMat.at<float>(i, 2);

                    if(confidence > confidenceThreshold)
                    {
                        int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols) - offset;
                        int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows) - offset;
                        int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols) + offset;
                        int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows) + offset;

                        dlib::rectangle rect((long)xLeftBottom,(long)yLeftBottom,(long)xRightTop,(long)yRightTop);
                        dets.push_back (rect);
                    }
                }
        return dets;
    }

private:
    float confidenceThreshold;
    dnn::Net net;
};


#endif // CNNDETECTOR_H
