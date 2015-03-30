#ifndef SERVER_DATA_HANDLER
#define SERVER_DATA_HANDLER

#include <string>

#include "config.hpp"
#include "request_parser.hpp"

class data_handler
{
public:
    data_handler();
    
    void process(const request_parser& rp, const config_storage& cfg, std::string& data);
    
private:
    bool process_config(const request_parser& rp, const config_storage& cfg, std::string& out_file_path);
    std::string response_error();
    std::string response_200(const std::string& input, const std::string& content_type);
};

#endif
