#include "router.hpp"

#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <unistd.h>

#include "cache_storage.hpp"
#include "blog_storage.hpp"
#include "template_storage.hpp"
#include "config.hpp"
#include "request_parser.hpp"
#include "data_handler.hpp"

struct work_data
{
    int sck;
    std::string data;
};

namespace router
{

int request_id = 0;
std::vector<work_data> work;
std::mutex mtx;
std::condition_variable cv;
bool has_data = false;
cache_storage cache;
blog_storage blogs("/home/cznp-server/blog.cfg");
config_storage cfg("/home/cznp-server/config.cfg", cache);
template_storage tpl("/home/cznp-server/templates.cfg");

void on_data(const int sck, const std::string& data)
{
    std::lock_guard<std::mutex> lock(mtx);
    has_data = true;
    
    work.emplace_back(work_data{ sck, data });

    cv.notify_all();
}

void run()
{
    std::cout << "[Router] Starting" << std::endl;
    
    std::vector<work_data> current_work;
    
    std::cout << "[Router] Started" << std::endl;
    
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]{ return has_data; });
            
            current_work.swap(work);
            
            has_data = false;
        }
        
        for(auto wrk : current_work)
        {
            request_id++;
            request_parser rp{request_id, wrk.data};
            
            std::cout << rp.to_string() << std::endl;
            
            if(rp.errors())
            {
                close(wrk.sck);
                continue;
            }
            
            std::string data;
            
            data_handler dh;
            dh.process(rp, cfg, cache, tpl, blogs, data);

            write(wrk.sck, data.c_str(), data.size());
            close(wrk.sck);
        }
        
        work.clear();
    }
}

}