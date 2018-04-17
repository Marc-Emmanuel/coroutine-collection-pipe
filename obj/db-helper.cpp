#include "db-helper.h"
#define DEBUG 1

DbHelper::DbHelper(){
    host = "mongodb://localhost";
    db_name = "testdb";
    port=27017;
    collectionName = "collection";
    mongocxx::instance inst{};
    uri = mongocxx::uri(host+":"+std::to_string(port));
    conn = mongocxx::client(uri);
    collection = conn[db_name][collectionName];
    count();
}

DbHelper::DbHelper(std::string _host, std::string _db, std::string _collection){
    _host = host;
    db_name = _db;
    port = 27017; // If defaut;
    collectionName = _collection;
    uri = mongocxx::uri(host+":"+std::to_string(port));
    conn = mongocxx::client(uri);
    collection = conn[db_name][collectionName]; // Connect to db and get Collection
    count();
}

//Set Collection to use and re get collection
void DbHelper::setCollection(std::string name){
    this->collectionName = name;
    this->collection = conn[this->db_name][name];
    count();
    #ifdef DEBUG
    std::cout <<"[DbHelper::setCollection]: " << this->db_name << "/" << this->collectionName << std::endl; 
    #endif
}
//Insert
void DbHelper::insert(std::string obj){
    bsoncxx::document::value doc_value = bsoncxx::from_json(obj);
    this->collection.insert_one(doc_value.view());
    this->count();
    #ifdef DEBUG
    std::cout << "[DbHelper::insert]: " << obj << std::endl;
    #endif
}

//Helper to string
const std::string DbHelper::toString(){
    std::stringstream ss;
    ss << host <<":"<<port<<"/"<<db_name<<"; collection: "<< collectionName << "; " << count() << " Documents";
    return ss.str();
}

//Get all documents in db/colname as a json vect
const std::vector<std::string> DbHelper::getAllDocuments(){
    auto cursor = this->collection.find({});
    std::vector<std::string> docs;
    for (auto&& doc : cursor) {
            docs.push_back(bsoncxx::to_json(doc));
    }
    this->count();
    return docs;
}

const int DbHelper::count(){
    auto cursor = this->collection.find({});
    int n = 0;
    for (auto&& doc : cursor) {
        n = n + 1;
    }
    doc_number = n;
    return n;
}