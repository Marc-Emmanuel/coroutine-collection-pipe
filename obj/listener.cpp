#include "listener.h"

Listener::Listener(){
    name = "(default)";
}

Listener::Listener(std::string n){
    name = n;
}

void Listener::OnEvent(){
    std::cout << "[" << name << "]: Received event" << std::endl;
}

void Listener::OnEvent(std::string mess){
    std::cout <<"[" << name << "]: Received event - " << mess << std::endl; 
}