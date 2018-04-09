#include "collector.h"

/*Collector();
Collector(std::string, int);
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
*/

Collector::Collector(){
    url = "http://jsonplaceholder.typicode.com/posts/1";
    frequency = 1; //s
    listener = Listener("Test listener");
}

Collector::Collector(std::string u, int freq){
    url = u;
    frequency = freq;
    listener = Listener("Test listener");
}

std::string Collector::Get(restc_cpp::Context& ctx){
    auto reply = ctx.Get(url);
    std::string s = reply->GetBodyAsString();
    return s;
}

void Collector::SetListener(Listener list){
    listener = list;
}

Listener Collector::GetListener() const {
    return listener;
}

void Collector::SetFrequency(int f){
    frequency = f;
}

int Collector::GetFrequency() const {
    return this->frequency;
}
std::string Collector::GetUrl() const {
    return this->url;
}
void Collector::SetUrl(std::string u){
    this->url = u;
}

void Collector::Stop(bool stop){
    run = stop;
}

void* Collector::InnerRun(void* inst){
    Collector *c = (Collector*) inst;
    auto rest_client = restc_cpp::RestClient::Create();
    while (c->GetReadyToRun()){
        // Call as a co-routine in a worker-thread.
        rest_client->Process([&](restc_cpp::Context& ctx) {
                std::string json = c->Get(ctx);
                c->GetListener().OnEvent(json);
        });
        sleep(c->GetFrequency());
    }
    rest_client->CloseWhenReady(true);
    pthread_exit(NULL);

}

bool Collector::GetReadyToRun() const {
    return this->run;
}
pthread_t Collector::Run(){
    void *status;
    run = true;
    pthread_create(&inner_thread, NULL, InnerRun, this);
    return inner_thread;
}
