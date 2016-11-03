#include <iostream>
#include <thread>
#include <memory>

#include "command_line.hpp"
#include "listener.hpp"
#include "router.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
    command_line cmd(argc, argv);
    if(cmd.has_error())
    {
        std::cout << "Error parsing command line arguments." << std::endl;
        return 1;
    }
    
    if(!cmd.has_argument_value("-path"))
    {
        std::cout << "Missing -path <directory>" << std::endl;
        return 1;
    }
    
    const std::string path = cmd.get_argument_value("-path");

    listener l;
    router r(path);
    
    l.request_callback(
        [&](const int sck, const std::string& data, const std::string& ip_address)
        {
            r.on_data(sck, data, ip_address);
        }
    );
    
    std::thread listener_thread(
        [&]()
        {
            l.start();
        }
    );
    
    std::thread router_thread(
        [&]()
        {
            r.run();
        }
    );
    
    listener_thread.join();
    router_thread.join();
    
    return 0;
}
