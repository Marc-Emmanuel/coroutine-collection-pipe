#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <restc-cpp/restc-cpp.h>
#include <restc-cpp/RequestBuilder.h>
#include <pthread.h>
#include "listener.h"

class Collector {
private:
    std::string url;
    int frequency;
    pthread_t inner_thread;
    Listener listener;
    static void* InnerRun(void*);
    bool run = false;
public:
    Collector();
    Collector(std::string, int);
    std::string Get(restc_cpp::Context&);
    void SetListener(Listener);
    Listener GetListener() const;
    void SetFrequency(int);
    int GetFrequency() const;
    std::string GetUrl() const;
    void SetUrl(std::string);
    bool GetReadyToRun()const;
    pthread_t Run();
    void Stop(bool);
};
