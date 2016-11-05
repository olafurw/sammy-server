#include "router.hpp"

using namespace std::chrono;

router::router(const std::string& path)
    : m_request_id(0)
    , m_has_data(false)
    , m_path(path)
    , m_storage(path)
{
}

router::~router()
{
}

void router::on_data(const int sck, const std::string& data, const std::string& ip_address)
{
    {
        std::unique_lock<std::mutex> lock(m_mtx);
    
        m_has_data = true;
        m_work.push_back(work_data{ sck, data, ip_address });
    }

    m_cv.notify_all();
}

void router::run()
{
    std::cout << "[Router] Starting" << std::endl;
    
    std::vector<work_data> current_work;
    m_update_timer = steady_clock::now();
    
    std::cout << "[Router] Started" << std::endl;
    
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_cv.wait(lock, [&]{ return m_has_data; });
            m_has_data = false;
            
            current_work.swap(m_work);
            m_work.clear();
        }
        
        for(auto wrk : current_work)
        {
            m_request_id++;
            
            request_parser rp{m_request_id, wrk.data, wrk.ip_address};
            
            m_storage.logger.log(rp.to_string());
            
            if(rp.errors())
            {
                close(wrk.sck);
                continue;
            }
            
            std::string data;
            
            data_handler dh(&m_storage);
            dh.process(rp, data);

            write(wrk.sck, data.c_str(), data.size());
            close(wrk.sck);
        }
        
        current_work.clear();
        
        steady_clock::time_point new_update = steady_clock::now();
        duration<double> diff = duration_cast<duration<double>>(new_update - m_update_timer);
        if(diff.count() >= 1.0)
        {
            m_storage.refresh();
            
            m_update_timer = new_update;
        }
    }
}
