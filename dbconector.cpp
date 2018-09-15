#include "dbconector.h"

DbConnector::DbConnector(std::string uriName)
{
    uri = mongocxx::uri(uriName);
    client = mongocxx::client(uri);
}


mongocxx::database DbConnector::getDatabase(std::string dbName)
{
    return client.database (dbName);
}
