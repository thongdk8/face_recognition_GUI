#ifndef FACEDATAMODEL_H
#define FACEDATAMODEL_H

#include "dbconector.h"
#include <bsoncxx/stdx/optional.hpp>
#include <dlib/matrix.h>
class MyFaceModel
{
public:
    MyFaceModel(DbConnector* dbConnector,std::string dbName, std::string faceCollectionName);

    bsoncxx::document::value createInstance(std::string name, dlib::matrix<float,0,1> descriptor, std::string date);
    bsoncxx::stdx::optional<mongocxx::result::insert_one> insertInstance(std::string name, dlib::matrix<float,0,1> descriptor, std::string date);
    mongocxx::collection getCollections();
private:
    mongocxx::collection coll;
};

#endif // FACEDATAMODEL_H
