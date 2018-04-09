#include "db-helper.h"
#define DEBUG

DbHelper::DbHelper(){
        host = "mongodb://localhost";
        db_name = "testdb";
        port=27017;
        collectionName = "collection";
        collection = conn[db_name][collectionName];
}

DbHelper::DbHelper(std::string _host, std::string _db, std::string _collection){
    _host = host;
    db_name = _db;
    port = 27017; // If defaut;
    collectionName = _collection;
    collection = conn[db_name][collectionName]; // Connect to db and get Collection
}

//Set Collection to use and re get collection
void DbHelper::SetCollection(std::string name){
        this->collectionName = name;
        this->collection = conn[this->db_name][name];
}
//Insert
void DbHelper::Insert(std::string obj){
        bsoncxx::document::value doc_value = bsoncxx::from_json(obj);
        this->collection.insert_one(doc_value.view());
        #ifdef DEBUG
        std::cout << "Inserted: " << obj << std::endl;
        #endif
}

//Helper to string
const std::string DbHelper::ToString(){
        std::stringstream ss;
        ss << host <<":"<<port<<"/"<<db_name<<"; collection: "<< collectionName;
        return ss.str();
}

//Get all documents in db/colname as a json vect
const std::vector<std::string> DbHelper::GetAllDocuments(){
        auto cursor = this->collection.find({});
        std::vector<std::string> docs;
        for (auto&& doc : cursor) {
                docs.push_back(bsoncxx::to_json(doc));
        }
        return docs;
}
