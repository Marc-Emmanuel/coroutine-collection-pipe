#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
//#include <thread>         // std::this_thread::sleep_for
//#include <chrono>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/algorithm/string.hpp>
#include <restc-cpp/restc-cpp.h>
#include <restc-cpp/RequestBuilder.h>
#include <pthread.h>
#include <map>
#include "listener.h"

class Collector {
private:
    std::string url;
    int frequency;
    pthread_t inner_thread;
    Listener * listener;
    static void* InnerRun(void*);
    bool isRunning = false;
    std::map<std::string, std::string> requestParameters;
    void parseUrl();
public:
    Collector();
    Collector(std::string, int);
    Collector(std::string, int, Listener *);
    std::string get(restc_cpp::Context&);
    void setListener(Listener * );
    Listener * getListener() const;
    void setFrequency(int);
    int getFrequency() const;
    std::string getUrl() const;
    void setUrl(std::string);
    bool shouldRun()const;
    pthread_t run();
    void join();
    void stop();
};
