#include "collector.h"
#define DEBUG 1
/*Collector();
Collector(std::string, int);
Collector(std::string, int, Listener);
std::string Get(restc_cpp::Context&);
void SetListener(Listener);
std::string GetListener() const;
void SetFrequency(int);
int GetFrequency() const;
std::string GetUrl() const;
void SetUrl(std::string);
bool Run();

std::string url;
int frequency;
std::pthread inner_thread;
Listener listener;
bool isRunning;
*/

Collector::Collector(){
    url = "http://jsonplaceholder.typicode.com/posts/1";
    frequency = 100 * 1000; //s | micro sec if usleep
    listener = new Listener("Test listener");
}

Collector::Collector(std::string u, int freq){
    url = u;
    frequency = freq;
    listener = new Listener("Test listener");
}

Collector::Collector(std::string u, int freq, Listener * list){
    url = u;
    frequency = freq;
    listener = list;
}

std::string Collector::get(restc_cpp::Context& ctx){
    auto reply = ctx.Get(url);
    std::string s = reply->GetBodyAsString();
    return s;
}

void Collector::setListener(Listener * list){
    listener = list;
}

Listener * Collector::getListener() const {
    return listener;
}

void Collector::setFrequency(int f){
    frequency = f;
}

int Collector::getFrequency() const {
    return this->frequency;
}
std::string Collector::getUrl() const {
    return this->url;
}
void Collector::setUrl(std::string u){
    this->url = u;
}

void Collector::stop(bool stop){
    isRunning = stop;
}

void* Collector::InnerRun(void* inst){
    Collector *c = (Collector*) inst;
    auto rest_client = restc_cpp::RestClient::Create();
    while (c->getReadyToRun()){
        // Call as a co-routine in a worker-thread.
        rest_client->Process([&](restc_cpp::Context& ctx) {
                std::string json = c->get(ctx);
                c->getListener()->onEvent(json);
        });
        // if using std::thread lib => std::this_thread::sleep_for(std::chrono::milliseconds(c->getFrequency()));
        usleep(c->getFrequency()); //Micro sec
    }
    rest_client->CloseWhenReady(true);
    pthread_exit(NULL);
}

bool Collector::getReadyToRun() const {
    return this->isRunning;
}
pthread_t Collector::run(){
    void *status;
    isRunning = true;
    pthread_create(&inner_thread, NULL, InnerRun, this);
    return inner_thread;
}
