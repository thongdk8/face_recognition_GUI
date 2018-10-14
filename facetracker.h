#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <string>
#include <utility>
#include <mutex>

class FaceTracker {
  public:
    FaceTracker(cv::Mat &frame, std::vector<cv::Rect> &bboxes,
                std::vector<std::string> &names, std::mutex mtx_frame, int tracker_id) {
        trackerType = trackerTypes[tracker_id];
        multiTracker = cv::MultiTracker::create();
        frame_ptr = &frame;
        this->full_bboxes = &bboxes;
        this->bnames = names;
        this->mtx_frame_ptr = mtx_frame;
    }

    ~FaceTracker() {
        delete bboxes;
        delete bnames;
    }

    void runTrackers() {
        while(true){
            mtx_frame_ptr->lock ();
            local_frame = frame_ptr->clone ();
            mtx_frame_ptr->unlock ();

            if(!local_frame.empty ()){
                multiTracker->update (local_frame);

                for(int i = multiTracker->getObjects ().size () - 1; i >=0; i--){
//                    cv::rectangle ()
                }
            }
        }

    }

    void addTracker(cv::Rect& rect, std::string& name) {

        bboxes->push_back (rect);
        bnames->push_back (name);

        cv::Ptr<cv::Tracker> tracker;
        if (trackerType == trackerTypes[0])
            tracker = TrackerBoosting::create();
        else if (trackerType == trackerTypes[1])
            tracker = TrackerMIL::create();
        else if (trackerType == trackerTypes[2])
            tracker = TrackerKCF::create();
        else if (trackerType == trackerTypes[3])
            tracker = TrackerTLD::create();
        else if (trackerType == trackerTypes[4])
            tracker = TrackerMedianFlow::create();
        else if (trackerType == trackerTypes[5])
            tracker = TrackerGOTURN::create();
        else if (trackerType == trackerTypes[6])
            tracker = TrackerMOSSE::create();
        else if (trackerType == trackerTypes[7])
            tracker = TrackerCSRT::create();
        else {
            cout << "Incorrect tracker name" << endl;
            cout << "Available trackers are: " << endl;
            for (vector<string>::iterator it = trackerTypes.begin();
                 it != trackerTypes.end(); ++it)
                std::cout << " " << *it << endl;
        }

        multiTracker->add(tracker, *frame_ptr, cv::Rect2d(rect));
    }

  public:
    cv::Ptr<cv::MultiTracker> multiTracker;
    cv::Ptr<cv::Mat> frame_ptr;
    cv::Mat local_frame;
    std::vector<cv::Rect> *bboxes;
    std::vector<std::string> *bnames;
    std::mutex* mtx_frame_ptr;

  private:
    std::vector<std::string> trackerTypes = {"BOOSTING", "MIL",        "KCF",
                                             "TLD",      "MEDIANFLOW", "GOTURN",
                                             "MOSSE",    "CSRT"};
    std::string trackerType;
};

#endif // FACETRACKER_H
