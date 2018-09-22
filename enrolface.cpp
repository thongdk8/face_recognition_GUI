//#include "mainwindow.h"
//#include <QApplication>
//#include <dbconector.h>
//#include <facedatamodel.h>

#include "datamanager.h"
//#include <defineddnn.h>
//#include <facerecognizer.h>

//#include <opencv2/highgui.hpp>


int main1(int argc, char *argv[])
{
    anet_type net;
    deserialize ("/home/thongpb/works/face_recognition/dlib-models-master/dlib_face_recognition_resnet_model_v1.dat") >> net;

    DataManager dm("para.txt");
    dm.loadData ("data.txt", "name_data.txt");
    dm.initSearchEngine ();

    dm.registerPerson (net, "/media/thongpb/EDisk/datasets/couple_faces/aligned_sync/vs/hien_VS/84.png", "The Hien");

    return 0;


}
