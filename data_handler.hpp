#ifndef SERVER_DATA_HANDLER
#define SERVER_DATA_HANDLER

#include <string>

#include "blog_storage.hpp"
#include "cache_storage.hpp"
#include "config.hpp"
#include "request_parser.hpp"

class data_handler
{
public:
    data_handler();
    
    void process(const request_parser& rp, const config_storage& cfg, const cache_storage& cache, const blog_storage& blogs, std::string& data);
    
private:
    bool process_static(const request_parser& rp, const cache_storage& cache, const config& cfg, std::string& data);
    bool process_dynamic(const request_parser& rp, const config& cfg, const blog_storage& blogs, std::string& data);
    bool process_json(const request_parser& rp, const config& cfg, std::string& data);
    std::string response_error();
    std::string response_200(const std::string& input, const std::string& content_type);
    std::string response_200_head(const int input_size, const std::string& content_type);
};

#endif
