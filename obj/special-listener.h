#pragma once
#include "listener.h"
#include "db-helper.h"

class SpecialListener: public Listener {
public: 
    SpecialListener();
    SpecialListener(std::string, DbHelper *);
    virtual void onEvent();
    virtual void onEvent(std::string);
    void log();
private:
    DbHelper * helper;
};