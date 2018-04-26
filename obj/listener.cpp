#include "listener.h"

Listener::Listener(){
    name = "(default)";
}

Listener::Listener(std::string n){
    name = n;
}

void Listener::onEvent(){
    mtx.lock();
    std::cout << "[" << name << "]: Received event" << std::endl;
    mtx.unlock();
}

void Listener::onEvent(std::string mess){
    mtx.lock();
    std::cout <<"[" << name << "]: Received event - " << mess << std::endl; 
    mtx.unlock();
}

void Listener::log(){
    std::cout <<"[" << name << "]" << std::endl;
}

void Listener::log(std::string m){
    std::cout <<"[" << name << "]: " << m << std::endl;
}