#include "special-listener.h"

SpecialListener::SpecialListener():Listener(){
    helper = new DbHelper();
}

SpecialListener::SpecialListener(std::string name, DbHelper * hp): Listener(name){
    helper = hp;
}

void SpecialListener::onEvent(){
    mtx.lock();
    Listener::onEvent(); //Parent void
    mtx.unlock();
}

void SpecialListener::onEvent(std::string mess){
    mtx.lock();
    std::string emptyObj = "{}";
    if(emptyObj.compare(mess)!=0){
        helper->insert(mess);
        this->log("Inserted in " + helper->getCollectionName());
    }
    mtx.unlock();
}

void SpecialListener::log(){
    std::cout <<"[" << name << "]" << std::endl;
}

void SpecialListener::log(std::string m){
    std::cout <<"[" << name << "]: " << m << std::endl;
}