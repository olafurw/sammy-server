#ifndef ROUTER
#define ROUTER

#include <functional>

#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <ratio>
#include <unistd.h>

#include "storage_handler.hpp"
#include "request_parser.hpp"
#include "data_handler.hpp"

struct work_data
{
    int sck;
    std::string data;
    std::string ip_address;
};

class router
{
public:
    router(const std::string& path);
    ~router();
    
    void on_data(const int sck, const std::string& data, const std::string& ip_address);
    void run();
    
private:
    std::string m_path;
    int m_request_id;
    std::vector<work_data> m_work;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_has_data;
    std::chrono::steady_clock::time_point m_update_timer;
    storage_handler m_storage;
    
};

#endif
