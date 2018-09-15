#include "mainwindow.h"
#include <QApplication>
#include <dbconector.h>
#include <facedatamodel.h>
int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    w.showFrame();
//    return a.exec();
    dlib::matrix<float,0,1> data;
    data.set_size (128);
    DbConnector* dbConnector = new DbConnector("mongodb://localhost:27017");
    MyFaceModel face_model_data(dbConnector, "faceDB", "face_collection");
    face_model_data.insertInstance ("testname",data,"12/09/2018");

    return 0;

}
