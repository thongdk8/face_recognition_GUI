#include "facedatamodel.h"

MyFaceModel::MyFaceModel(DbConnector *dbConnector, std::string dbName, std::string faceCollectionName)
{
    mongocxx::database db = dbConnector->getDatabase (dbName);
    coll = (db[faceCollectionName]);
}


bsoncxx::document::value MyFaceModel::createInstance (std::__cxx11::string name, dlib::matrix<float,0,1> descriptor, std::__cxx11::string date)
{
    auto builder = bsoncxx::builder::stream::document{};

    builder << "name" << name << "date" << date;

    auto data_array_builder = builder << "data" << open_array;

    for(long r = 0; r < descriptor.nr (); ++r)
        for(long c = 0; c < descriptor.nc (); ++c)
        {
            data_array_builder << descriptor(r,c) ;
        }
    data_array_builder << close_array;

//    bsoncxx::document::value doc_value = builder
//      << "name" << name
//      << "date" << date
//      << "versions" << bsoncxx::builder::stream::open_array
//        << "v3.2" << "v3.0" << "v2.6"
//      << close_array
//      << bsoncxx::builder::stream::finalize;

    bsoncxx::document::value doc_value = builder << bsoncxx::builder::stream::finalize;
    std::cout << "Logging from MyFaceModel: " << bsoncxx::to_json (doc_value) << std::endl;
    return std::move(doc_value);
}

bsoncxx::stdx::optional<mongocxx::result::insert_one> MyFaceModel::insertInstance (std::__cxx11::string name, dlib::matrix<float,0,1> descriptor, std::__cxx11::string date)
{
    auto doc = createInstance (name, descriptor, date);
    bsoncxx::stdx::optional<mongocxx::result::insert_one> res = coll.insert_one(std::move(doc));
    return res;
}
