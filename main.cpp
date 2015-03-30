#include <iostream>
#include <thread>
#include <memory>

#include "listener.hpp"
#include "router.hpp"

void on_data(const int sck, const std::string& data)
{
    router::on_data(sck, data);
}
int main()
{
    listener l;
    l.request_callback(on_data);
    std::thread listener_thread(&listener::start, l);
    std::thread router_thread(router::run);
    
    listener_thread.join();
    router_thread.join();
    
    return 0;
}
