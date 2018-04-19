#include "special-listener.h"

SpecialListener::SpecialListener():Listener(){
    helper = new DbHelper();
}

SpecialListener::SpecialListener(std::string name, DbHelper * hp): Listener(name){
    helper = hp;
}

void SpecialListener::onEvent(){
    Listener::onEvent(); //Parent void
}

void SpecialListener::onEvent(std::string mess){
    helper->insert(mess);
    this->log("Inserted in " + helper->getCollectionName());
}

void SpecialListener::log(){
    std::cout <<"[" << name << "]" << std::endl;
}

void SpecialListener::log(std::string m){
    std::cout <<"[" << name << "]: " << m << std::endl;
}