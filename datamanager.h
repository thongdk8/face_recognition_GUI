#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

#include <queue>
#include <chrono>
#include "hnswlib/hnswlib.h"
#include <unordered_set>
#include <map>

#include <unistd.h>
#include <sys/resource.h>
#include <vector>
#include <dlib/matrix.h>

#include <glob.h>
#include <dlib/image_io.h>
#include "facerecognizer.h"

using namespace std;
using namespace hnswlib;

class DataManager {
public:
    DataManager()
    {
        this->para_fn = "para.txt";
    }

    DataManager(std::string para_fn){
        fstream para_file;
        para_file.open (para_fn);
        para_file >> vecdim;
        para_file >> nb_instance;
        para_file >> nb_class;
        para_file >> knn;
        para_file >> threshold;
        para_file >> majority_threshold;
        para_file.close ();

        knn_threshold = majority_threshold*knn;

        data = new float[vecdim*nb_instance];
        id = new int[nb_instance];
        names = new std::string[nb_class];
        data_query = new float[vecdim];
        max_instance = 10000;

        this->para_fn = para_fn;
    }

    void loadData(std::string data_fn, std::string name_data_fn){
        ifstream data_file, name_data_file;
        data_file.open (data_fn);
        name_data_file.open (name_data_fn);

        for(int i=0; i< nb_class; i++){
            std::string tempstr;
            std::getline(name_data_file, tempstr);
            names[i] = tempstr;
        }

        for(int i=0; i<nb_instance; i++){
            int tmpid;
            data_file >> tmpid;
            id[i] = tmpid;
            for(int j=0; j < vecdim; j++){
                float tmp;
                data_file >> tmp;
                data[i*vecdim+j] = tmp;
            }
        }

        name_data_file.close ();
        data_file.close ();

        this->data_fn = data_fn;
        this->name_data_fn = name_data_fn;

        cout << "Loading data completed!!" << endl;
    }


    void writeWholeData(){
        ofstream data_file, name_data_file;
        data_file.open (data_fn);
        name_data_file.open (name_data_fn);

        for(int i=0; i< nb_class; i++){
            name_data_file << names[i] << "\n";
        }

        for(int i=0; i<nb_instance; i++){
            data_file << id[i] << " ";
            for(int j=0; j < vecdim; j++){
                data_file << data[i*vecdim+j] << " ";
            }
        }

        name_data_file.close ();
        data_file.close ();
        cout << "Write whole data completed!!" << endl;
    }

    void initSearchEngine(){
        l2space = new L2Space(vecdim);
        appr_alg = new HierarchicalNSW<float> (l2space, max_instance, 16, 200);
        for(int i = 0; i < nb_instance; i++){
            appr_alg->addPoint ( (void*)(data + vecdim*i), (size_t) id[i]  );
        }
    }

    std::string getInstanceName(float* instance){
        bool isExisted = false;

        std::priority_queue<std::pair<float, labeltype >> answers;
        float threshold2 = threshold*threshold;
        int count_true = 0;
        std::map<size_t, int> label_counter;
        answers = appr_alg->searchKnn(instance,knn);
        while(!answers.empty ()){
//        for(auto ans : answers){
//        for(std::priority_queue<std::pair<float, labeltype >>::iterator ans = answers.)

            auto ans = answers.top ();
            answers.pop();
//            std::cout << "distance: " << ans.first << std::endl;
            if(ans.first <= threshold2){
                count_true ++;
                label_counter[ans.second]++;
//                std::cout << "label_counter[" << ans.second << "] = " << label_counter[ans.second] << std::endl;
            }
        }

        std::cout << "Count true: " << count_true << std::endl;

        int max_label = -1;
        size_t max_label_idx = -1;
        if(count_true >= knn_threshold){
            for (auto lbc : label_counter){
                if(lbc.second > max_label){
                    max_label_idx = lbc.first;
                    max_label = lbc.second;
                }

            }
        }
        std::cout << "label_counter[" << max_label_idx << "] = " << label_counter[max_label_idx] << std::endl;
        if(count_true > 0){
            if(label_counter[max_label_idx]/count_true >= majority_threshold)
                isExisted = true;
        }


        if(isExisted)
            return names[max_label_idx];
        else
            return "Unknow";
    }

    std::vector<std::string> getInstanceName (std::vector<dlib::matrix<float,0,1> > face_descriptors)
    {
        std::vector<std::string> instanceNames;
        float des_array[128];
        if(face_descriptors.size ()>0){
            for(auto des : face_descriptors)
            {
                  for (long c = 0; c < 128; ++c)
                  {
                      des_array[c] = des(0,c);
                  }

                  auto name_rt = getInstanceName (des_array);
                  instanceNames.push_back (name_rt);
            }
        }

        return instanceNames;
    }

    bool checkIsExisted(float* instance)
    {
        bool isExisted = false;

        std::priority_queue<std::pair<float, labeltype >> answers;
        float threshold2 = threshold*threshold;
        int count_true = 0;
        std::map<size_t, int> label_counter;
        answers = appr_alg->searchKnn(instance,knn);

        while (!answers.empty ()) {
//        for(auto ans : answers){
            auto ans = answers.top ();
            answers.pop();
            if(ans.first <= threshold2){
                count_true ++;
                label_counter[ans.second]++;
            }
        }

        int max_label = -1;
        size_t max_label_idx = -1;
        if(count_true >= knn_threshold){
            for (auto lbc : label_counter){
                if(lbc.second > max_label){
                    max_label_idx = lbc.first;
                    max_label = lbc.second;
                }

            }
        }

        if(max_label!=-1 && max_label_idx!=-1){
            if(label_counter[max_label_idx]/count_true >= majority_threshold)
                isExisted = true;
        }

        return isExisted;

    }

    int addAnInstanceForced(float* instance, std::string name){
        appr_alg->addPoint ( (void*) instance, size_t(nb_class));
        ofstream data_file;
        data_file.open (data_fn, ios::app);
        data_file << nb_class << " ";
        for(int i=0; i< vecdim; i++){
            data_file << instance[i] << " ";
        }
        data_file.close ();

        ofstream name_data_file;
        name_data_file.open(name_data_fn, ios::app);
        name_data_file << name << "\n";
        name_data_file.close ();

        nb_instance++;
        nb_class++;
        updataParam (this->para_fn);

        return nb_class-1;
    }

    int addAnInstanceForced(float* instance, std::string name, int id_face){
        appr_alg->addPoint ( (void*) instance, size_t(id_face));
        ofstream data_file;
        data_file.open (data_fn, ios::app);
        data_file << id_face << " ";
        for(int i=0; i< vecdim; i++){
            data_file << instance[i] << " ";
        }
        data_file.close ();

//        ofstream name_data_file;
//        name_data_file.open(name_data_fn, ios::app);
//        name_data_file << name << "\n";
//        name_data_file.close ();

        nb_instance++;
//        nb_class++;
        updataParam (this->para_fn);

        return nb_class-1;
    }

    int addAnInstanceWithNotExisted(float* instance, std::string name){

        int id_face = -1;
        bool isExisted = false;

        isExisted = checkIsExisted (instance);

        /*std::priority_queue<std::pair<float, labeltype >> answers;
        float threshold2 = threshold*threshold;
        int count_true = 0;
        std::map<size_t, int> label_counter;
        answers = appr_alg->searchKnn(instance,knn);

        while (!answers.empty ()) {
//        for(auto ans : answers){
            auto ans = answers.top ();
            answers.pop();
            if(ans.first <= threshold2){
                count_true ++;
                label_counter[ans.second]++;
            }
        }

        int max_label = -1;
        size_t max_label_idx = -1;
        if(count_true >= knn_threshold){
            for (auto lbc : label_counter){
                if(lbc.second > max_label){
                    max_label_idx = lbc.first;
                    max_label = lbc.second;
                }

            }
        }

        if(max_label!=-1 && max_label_idx!=-1){
            if(label_counter[max_label_idx]/count_true >= majority_threshold)
                isExisted = true;
        }*/


        if(isExisted)
            cout << "This face is exited in database!!" << endl;

        else
        {
            /*
            appr_alg->addPoint ( (void*) instance, size_t(id[nb_class-1]+1));
            ofstream data_file;
            data_file.open (data_fn, ios::app);
            data_file << id[nb_class-1]+1 << " ";
            for(int i=0; i< vecdim; i++){
                data_file << instance[i] << " ";
            }
            data_file.close ();

            ofstream name_data_file;
            name_data_file.open(name_data_fn, ios::app);
            name_data_file << name << "\n";
            name_data_file.close ();

            nb_instance++;
            nb_class++;
            updataParam (this->para_fn);
            */
            id_face = addAnInstanceForced (instance, name);
        }

        return id_face;

    }

    void updataParam(std::string para_fn){
        ofstream para_file;
        para_file.open (para_fn);
        para_file << vecdim << "\n";
        para_file << nb_instance<< "\n";
        para_file << nb_class<< "\n";
        para_file << knn<< "\n";
        para_file << threshold<< "\n";
        para_file << majority_threshold<< "\n";
        para_file.close ();
    }

    int registerPerson(anet_type& net, std::string image_path,
                        std::string name, bool isForcing = false, int id_face = -1)
    {
        int rt_id = -1;
        dlib::matrix<dlib::rgb_pixel> face_aligned;
        dlib::load_image(face_aligned, image_path);
        std::vector<matrix<rgb_pixel>> faces;
        faces.push_back (face_aligned);
        auto face_descriptors = net(faces);

        float des_array[128];
        if(face_descriptors.size ()>0){
            for(auto des : face_descriptors)
            {
                  for (long c = 0; c < 128; ++c)
                  {
                      des_array[c] = des(0,c);
                  }

                  if(isForcing){
                      if (id_face == -1)
                          rt_id = addAnInstanceForced (des_array, name);
                      else
                          rt_id = addAnInstanceForced (des_array, name, id_face);
                  }
                  else{
                      rt_id = addAnInstanceWithNotExisted (des_array, name);
                  }
            }
        }

        return rt_id;

    }

    ~DataManager(){
        delete[] data;
        delete[] data_query;
        delete l2space;
        delete appr_alg;
    }

public:
    float* data;
    int* id;
    std::string* names;
    float* data_query;

    int vecdim;
    int nb_instance;
    int nb_class;
    int knn;
    int knn_threshold;
    float threshold;
    float majority_threshold;

    int max_instance;

    L2Space* l2space;
    HierarchicalNSW<float>* appr_alg;

    std::string para_fn;
    std::string data_fn;
    std::string name_data_fn;
};

//std::vector<string> globVector(const string& pattern){
//    glob_t glob_result;
//    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
//    std::vector<string> files;
//    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
//        files.push_back(string(glob_result.gl_pathv[i]));
//    }
//    globfree(&glob_result);
//    return files;
//}


#endif // DATAMANAGER_H
