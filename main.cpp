#define BOOST_LOG_DYN_LINK 1
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <restc-cpp/restc-cpp.h>
#include <restc-cpp/RequestBuilder.h>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/config/prelude.hpp>
#include <bsoncxx/types.hpp>
#include "pthread.h"
#include "obj/db-helper.h"
#include "obj/collector.h"
#include "obj/listener.h"
#include "obj/special-listener.h"

using namespace std;
using namespace restc_cpp;
namespace logging = boost::log;

int main() {
        //Have to be called only once!
        mongocxx::instance inst{};

        logging::core::get()->set_filter
        (
                logging::trivial::severity >= logging::trivial::info
        );

        DbHelper * helper = new DbHelper();
        DbHelper * weatherHelper = new DbHelper("mongodb://localhost", "testdb","weather_data");
        helper->log();
        
        Listener * spListener = new SpecialListener("Insert Listener", helper);
        Listener * weatherDataReceiver = new SpecialListener("WeatherData Listener", weatherHelper);
        Collector testCollector;
        testCollector.setListener(spListener);

        Collector weatherCollector("http://api.openweathermap.org/data/2.5/weather?q=Paris,fr&appid=your-app-id", 30*1000*1000, weatherDataReceiver);
    
        weatherCollector.run();
        testCollector.run();
        cout << "[Collector::join]: " << "waiting for Collector to be destroyed" << endl;
        cout << "[Collector::join]: " << "waiting for WeatherCollector to be destroyed" << endl;
        weatherCollector.join();
        testCollector.join();
        //pthread_join(t, NULL);


        // auto rest_client = RestClient::Create();
        //
        // // Call DoSomethingInteresting as a co-routine in a worker-thread.
        // //rest_client->Process(helper.PerformGet);
        // rest_client->Process([&](Context& ctx) {
        //         helper.PerformGet(ctx);
        // });
        // Wait for the coroutine to finish, then close the client.
        //std::cout<<"Waiting for the client to close the connection"<<std::endl;
        //rest_client->CloseWhenReady(true);
        return 0;

}
