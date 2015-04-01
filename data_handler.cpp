#include "data_handler.hpp"

#include "database.hpp"

#include "libraries/json.hpp"
#include "libraries/format.h"

data_handler::data_handler()
{

}

void data_handler::process(const request_parser& rp, const config_storage& cfg, std::string& data)
{
    config request_config;
    if(!cfg.get(rp.get_path(), request_config))
    {
        data = response_error();
        return;
    }
    
    if(request_config.method != rp.get_method())
    {
        data = response_error();
        return;
    }
    
    if(request_config.type == type_static
       && !process_static(rp, request_config, data))
    {
        data = response_error();
        return;
    }
    
    if(request_config.type == type_json
       && !process_json(rp, request_config, data))
    {
        data = response_error();
        return;
    }
}

bool data_handler::process_static(const request_parser& rp, const config& cfg, std::string& data)
{
    if(!utils::file_exists(cfg.location))
    {
        return false;
    }
    
    if(rp.is_data_requested())
    {
        const std::string file_data = utils::file_to_string(cfg.location);
        data = response_200(file_data, cfg.mimetype);
    }
    else
    {
        const int file_size = utils::file_size(cfg.location);
        data = response_200_head(file_size, cfg.mimetype);
    }
    
    return true;
}

bool data_handler::process_json(const request_parser& rp, const config& cfg, std::string& data)
{
    nlohmann::json request_json;
    try
    {
        request_json = nlohmann::json::parse(rp.get_post_data());
    }
    catch(...)
    {
        return false;
    }
    
    database db;
    if(!db.increment_button())
    {
        return false;
    }
    
    unsigned int counter = 0;
    if(!db.get_button_counter(counter))
    {
        return false;
    }
    
    nlohmann::json reply_json;
    reply_json["counter"] = counter;
    
    data = response_200(reply_json.dump(), cfg.mimetype);
    
    return true;
}

std::string data_handler::response_error()
{
    return R"header(HTTP/1.0 404 Not Found
Server: sammy v0.5
MIME-version: 1.0
Content-type: text/plain
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: 28

Requested content not found!

)header";
}

std::string data_handler::response_200(const std::string& input, const std::string& content_type)
{
    //Set-Cookie: LOL_SESSION=abc123; Expires=Wed, 16 Nov 2015 12:34:56 GMT
    std::string header = R"header(HTTP/1.0 200 OK
Server: sammy v0.5
MIME-version: 1.0
Content-type: {0}
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: {1}

{2}

)header";
    
    return fmt::format(header, content_type, input.size(), input);
}

std::string data_handler::response_200_head(const int input_size, const std::string& content_type)
{
    //Set-Cookie: LOL_SESSION=abc123; Expires=Wed, 16 Nov 2015 12:34:56 GMT
    std::string header = R"header(HTTP/1.0 200 OK
Server: sammy v0.5
MIME-version: 1.0
Content-type: {0}
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: {1}

)header";
    
    return fmt::format(header, content_type, input_size);
}