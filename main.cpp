#include "mainwindow.h"
#include <QApplication>

//#include <dbconector.h>
//#include <facedatamodel.h>
#include "datamanager.h"

#include <thread>
//#include <mutex>
#include <chrono>

cv::Mat frame;
std::mutex fr_mutex;
cv::VideoCapture cap;

void readFrame(std::string camURL, int id = -1) {
    if (id == -1) {
        cap.open(camURL);
    } else
        cap.open(id);

    Mat frame_tmp;
    while (cap.isOpened()) {

        cap.read(frame_tmp);
        fr_mutex.lock();
        frame = frame_tmp.clone();
        fr_mutex.unlock();
        //        std::this_thread::sleep_for(std::chrono::seconds(1));
        //        std::cout << "In thread ....." << std::endl;
    }
    if(cap.isOpened ())
    {
        cap.release ();
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    int camID = w.camID;
    if (w.isIPCam)
        camID = -1;
    std::thread readFrameThread(readFrame, w.camURL, camID);

    w.show();
    //    w.showFrame();
    w.showFrame(fr_mutex, frame, cap);

//    readFrameThread.join();
    readFrameThread.detach ();
    return a.exec();

    //    dlib::matrix<float,0,1> data;
    //    data.set_size (128);
    //    DbConnector* dbConnector = new
    //    DbConnector("mongodb://localhost:27017");
    //    MyFaceModel face_model_data(dbConnector, "faceDB", "face_collection");
    //    face_model_data.insertInstance ("testname",data,"12/09/2018");

    //    size_t dim = 128;
    //    size_t num_element = 10;
    //    int* id = new int[num_element];

    //    for(int i =0; i<num_element; i++)
    //        id[i] = 0;

    //    float* query = new float[128];
    //    for (int i =0; i< 128; i++){
    //        query[i] = i;
    //    }
    //    float* mass = new float[num_element*dim];
    //    for (int i=0; i< 128;i++){
    //        if (i==127){
    //            mass[i] = 100;
    //        }
    //        else if (i==126)
    //            mass[i] = 100;
    //        else
    //        mass[i] = i;
    //    }

    //    for (int i = 128; i< num_element*dim; i++)
    //    {
    //        mass[i] = i;
    //    }
    //    std::string* names = new string[1];
    //    names[0] = "Testing";
    //    DataManager dm;
    //    dm.data = mass;
    //    dm.id = id;
    //    dm.vecdim = dim;
    //    dm.names = names;
    //    dm.nb_instance = 10;
    //    dm.nb_class = 1;
    //    dm.threshold = 0.6;
    //    dm.threshold = 0.6;
    //    dm.data_fn = "data.txt";
    //    dm.name_data_fn = "name_data.txt";

    /*
        DataManager dm("para.txt");
        dm.loadData ("data.txt", "name_data.txt");
        dm.initSearchEngine ();

    //    dm.writeWholeData ();
        float * instance = new float[128];
        for(int i =0; i<dim; i++)
            instance[i] = i;
        instance[126] = 100;
        instance[127] = 100;
        dm.addAnInstance (instance, "testing2");

        std::string res = dm.getInstanceName (instance);
        cout << res << endl;
    //    dm.vecdim = 128;
    //    dm.nb_instance = 2;
    //    dm.nb_class = 1;
    //    dm.knn = 1;
    //    dm.threshold = 0.6;
    //    dm.majority_threshold = 0.6;
    //    dm.updataParam (dm.para_fn);
        return 0;
    */
}
