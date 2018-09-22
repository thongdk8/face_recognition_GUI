#include "mainwindow.h"
#include <QApplication>
//#include <dbconector.h>
//#include <facedatamodel.h>

#include "datamanager.h"

int registerFace(char* argv[])
{
    cout << argv[1] << endl;
    anet_type net;
    deserialize (argv[1]) >> net;

    DataManager dm("para.txt");
    dm.loadData ("data.txt", "name_data.txt");
    dm.initSearchEngine ();

    std::vector<std::string> namess;
    std::vector<int> ids;
    int nb_faces = -1;


    fstream meta_data;
    meta_data.open ("meta_data_face.txt");
    meta_data >> nb_faces;

    cout << "read data, nb_faces " << nb_faces << endl;

    for (int i =0; i<nb_faces; i++)
    {
        int auto_id = -1;
        std::string name;
        cout << "read name" << endl;
        getline (meta_data, name);
        getline(meta_data, name);
        cout << "name" << name << endl;
        int id;
        meta_data >> id;
        int nb_im;
        meta_data >> nb_im;
        for (int j=0; j< nb_im; j++){
            std::string path;
            meta_data >> path;
            std::cout << "path" <<  path << std::endl;
            if (j==0)
                auto_id = dm.registerPerson (net, path, name, true);
            else
                dm.registerPerson(net, path, name, true, auto_id);
            cout << "registered id = " << auto_id << endl;
        }
        // dm.nb_class ++;
        // dm.updataParam (dm.para_fn);
    }
    return 0;

    return 0;
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.showFrame();

    return a.exec();



//    dlib::matrix<float,0,1> data;
//    data.set_size (128);
//    DbConnector* dbConnector = new DbConnector("mongodb://localhost:27017");
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
