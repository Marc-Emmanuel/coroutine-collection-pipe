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
    DbHelper(std::string, std::string, std::string); //host:port , dbname
    void SetCollection(std::string);
    void Insert(std::string);
    const std::string ToString();
    const std::vector<std::string>GetAllDocuments();
private:
    std::string host;
    std::string db_name;
    int port;
    std::string collectionName;
    mongocxx::v_noabi::collection collection;
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};
};
