#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

class Listener{
protected:
    std::string name;
public:
    Listener();
    Listener(std::string);
    virtual void onEvent();
    virtual void onEvent(std::string);
    virtual void log();
    virtual void log(std::string);
};
