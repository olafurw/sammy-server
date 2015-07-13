#ifndef SERVER_DATA_HANDLER
#define SERVER_DATA_HANDLER

#include <string>
#include <sstream>

#include "storage_handler.hpp"
#include "request_parser.hpp"

class data_handler
{
public:
    data_handler(const storage_handler* storage);
    void process(const request_parser& rp, std::string& data);
    
private:
    bool process_static(std::string& data);
    bool process_blog(std::string& data);
    bool process_blog_list(std::string& data);
    bool process_template(std::string& data);
    bool process_dynamic(std::string& data);
    bool process_json(std::string& data);
    std::string response_error();
    std::string response_200(const std::string& input, const std::string& content_type);
    std::string response_200_head(const int input_size, const std::string& content_type);
    
    const storage_handler* m_storage;
    const request_parser* m_request;
    config m_request_config;
    const std::string m_data;
};

#endif
