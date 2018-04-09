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

using namespace std;
using namespace restc_cpp;
namespace logging = boost::log;

int main() {
        logging::core::get()->set_filter
        (
                logging::trivial::severity >= logging::trivial::info
        );

        DbHelper helper;
        cout << "Helper: " << helper.ToString() <<endl;

        Listener listener("Test listener");
        Collector testCollector;
        testCollector.SetListener(listener);
        pthread_t t = testCollector.Run();
        cout << "Waiting for Collector to be destroyed" << endl;
        pthread_join(t, NULL);


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
