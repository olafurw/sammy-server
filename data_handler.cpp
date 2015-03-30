#include "data_handler.hpp"

#include "libraries/format.h"

data_handler::data_handler()
{

}

void data_handler::process(const request_parser& rp, const config_storage& cfg, std::string& data)
{
    std::string file_path;
    if(!process_config(rp, cfg, file_path))
    {
        data = response_error();
        return;
    }
    
    const std::string content_type = utils::filename_to_mimetype(file_path);
    const std::string file_data = utils::file_to_string(file_path);
    
    data = response_200(file_data, content_type);
}

bool data_handler::process_config(const request_parser& rp, const config_storage& cfg, std::string& out_file_path)
{
    config request_config;
    if(!cfg.get(rp.get_path(), request_config) && !cfg.get_folder(rp.get_path(), request_config))
    {
        return false;
    }
    
    if(request_config.method == route_method::method_get && rp.get_method() != "GET")
    {
        return false;
    }
    
    if(request_config.type == type_static)
    {
        if(!utils::file_exists(request_config.location))
        {
            return false;
        }
        
        out_file_path = request_config.location;
        return true;
    }
    
    if(request_config.type == type_static_folder)
    {
        const std::string file_path = request_config.location + rp.get_path();
        if(!utils::file_exists(file_path))
        {
            return false;
        }
        
        out_file_path = file_path;
        return true;
    }
    
    return false;
}

std::string data_handler::response_error()
{
    return R"header(HTTP/1.0 404 Not Found
Server: sammy v0.5
MIME-version: 1.0
Content-type: text/plain
Last-Modified: Thu 1 Jan 1970 00:00:00 PM EST
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
Last-Modified: Thu 1 Jan 1970 00:00:00 PM EST
Content-Length: {1}

{2}

)header";
    
    return fmt::format(header, content_type, input.size(), input);
}