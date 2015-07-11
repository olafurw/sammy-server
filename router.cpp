#include "router.hpp"

#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <unistd.h>

#include "storage_handler.hpp"
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

storage_handler storage("/home/cznp-server/");

void on_data(const int sck, const std::string& data)
{
    mtx.lock();
    has_data = true;
    
    work.push_back(work_data{ sck, data });
    
    mtx.unlock();

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
            has_data = false;
            
            current_work.swap(work);
            work.clear();
        }
        
        for(auto wrk : current_work)
        {
            request_id++;
            
            request_parser rp{request_id, wrk.data};
            
            storage.logger.log(rp.get_identifier(), rp.to_string());
            
            if(rp.errors())
            {
                close(wrk.sck);
                continue;
            }
            
            std::string data;
            
            data_handler dh(&storage);
            dh.process(rp, data);

            write(wrk.sck, data.c_str(), data.size());
            close(wrk.sck);
        }
        
        storage.refresh();
        
        current_work.clear();
    }
}

}