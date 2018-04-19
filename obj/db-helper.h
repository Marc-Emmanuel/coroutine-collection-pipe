#pragma once
#include <iostream>
#include <sstream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/exception/error_code.hpp>
#include <mongocxx/exception/logic_error.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/exception/server_error_code.hpp>
#include <vector>

//DB Data feeder -> Needs refactoring
class DbHelper {
public:
    DbHelper(); //default
    DbHelper(std::string, std::string, std::string); //host:port , dbname, collection
    void setCollection(std::string);
    void insert(std::string);
    const std::string toString();
    std::string getCollectionName() const;
    const std::vector<std::string>getAllDocuments();
    const int count();
    void log();
private:
    std::string host;
    std::string db_name;
    int port;
    std::string collectionName;
    int doc_number;
    mongocxx::v_noabi::collection collection;
    mongocxx::client conn;
    mongocxx::uri uri;
};
