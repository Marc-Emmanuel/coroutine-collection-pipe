#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

class Listener{
private:
    std::string name;
public:
    Listener();
    Listener(std::string);
    void OnEvent();
    void OnEvent(std::string);
};
