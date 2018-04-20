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
    frequency = 1000 * 1000; //s | micro sec if usleep
    listener = new Listener("Test listener");
    parseUrl();
}

Collector::Collector(std::string u, int freq){
    url = u;
    frequency = freq;
    listener = new Listener("Test listener");
    parseUrl();
}

Collector::Collector(std::string u, int freq, Listener * list){
    url = u;
    frequency = freq;
    listener = list;
    parseUrl();
}
//Parse Url to extract parameters if any, very important method
void Collector::parseUrl(){
    size_t pos = 0;
    std::string delimiter = "?";
    
    //remove what is before the first ?
    std::string urlParams = this->url.substr(this->url.find(delimiter) + delimiter.length());;
    std::vector<std::string> parameterPairs;
     //Clear map in case Collector is being reused:
    requestParameters.clear();
    
    if(this->url.compare(urlParams) == 0){
        //If no parameters, exit
        return;
    }
    
    this->url = this->url.substr(0, this->url.find(delimiter));
    
    //Extract parameters
    boost::split(parameterPairs, urlParams, boost::is_any_of("&"));
    //Store request parameters in the parameters Map
    for(std::vector<std::string>::size_type i = 0; i != parameterPairs.size(); i++) {
        std::vector<std::string> param;
        boost::split(param, parameterPairs[i], boost::is_any_of("="));
        std::vector<std::string>::size_type t = 0;
        requestParameters[param[t]]=param[t+1];
    }
    
    //log purpose
    #ifdef DUMPOBJ
    std::map<std::string, std::string>::iterator it;
    for ( it = requestParameters.begin(); it != requestParameters.end(); it++ ){
        std::cout << it->first  // string (key)
              << "->"
              << it->second   // string's value 
              << std::endl ;
    }
    #endif
}

std::string Collector::get(restc_cpp::Context& ctx){
    std::string s ;
    restc_cpp::RequestBuilder * r = new restc_cpp::RequestBuilder(ctx);
    r->Get(url);
    //If some parameters have been extracted, add them to the Request Builder
    if(!requestParameters.empty()){
        std::map<std::string, std::string>::iterator it;
        for ( it = requestParameters.begin(); it != requestParameters.end(); it++ ){
            r->Argument(it->first, it->second);   // Key, Value 
        }
    }
    try{
        auto reply = r->Execute();
        delete r;
        r = NULL;
        s = reply->GetBodyAsString();
    } catch(const std::exception& ex){
        #ifdef DEBUG
        std::cout << "[Collector::get]: "<< ex.what() << " has been caught, sending empty object {}" << std::endl;
        #endif
        s="{}";
    }
    return s;
}

void Collector::setListener(Listener * list){
    #ifdef DEBUG
    std::cout <<"[Collector::setListener]: " << this->url << ", request frequency: " << this->frequency << ", Listener: " ; 
    list->log();
    #endif
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
    parseUrl(); //New url using the same collector. Will be removed in future work
}

void Collector::stop(bool stop){
    isRunning = stop;
}

void Collector::join(){
    #ifdef DEBUG
    std::cout <<"[Collector::join]: " << this->url << ", request frequency: " << this->frequency << std::endl; 
    #endif
    pthread_join(inner_thread, NULL);
}
void* Collector::InnerRun(void* inst){
    Collector *c = (Collector*) inst;
    auto rest_client = restc_cpp::RestClient::Create();
    #ifdef DEBUG
    std::cout <<"[Collector::InnerRun]: " << c->url << ", request frequency: " << c->frequency << ", Listener: ";
    c->getListener()->log(); 
    #endif
    while (c->shouldRun()){
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

bool Collector::shouldRun() const {
    #ifdef DEBUG
    std::cout <<"[Collector::shouldRun]: " << this->url << ", request frequency: " << this->frequency << ", running: " << this->isRunning << std::endl; 
    #endif
    return this->isRunning;
}

pthread_t Collector::run(){
    void *status;
    isRunning = true;
    #ifdef DEBUG
    std::cout <<"[Collector::run]: " << this->url << ", request frequency: " << this->frequency << std::endl; 
    #endif
    pthread_create(&inner_thread, NULL, InnerRun, this);
    return inner_thread;
}
