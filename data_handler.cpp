#include "data_handler.hpp"

#include "libraries/json.hpp"
#include "libraries/format.h"

#include "utils.hpp"

data_handler::data_handler()
{

}

void data_handler::process(const request_parser& rp, const config_storage& cfg, const cache_storage& cache, const template_storage& tpl, const blog_storage& blogs, std::string& data)
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
       && !process_static(rp, cache, request_config, data))
    {
        data = response_error();
        return;
    }
    
    if(request_config.type == type_dynamic
       && !process_dynamic(rp, request_config, blogs, data))
    {
        data = response_error();
        return;
    }
    
    if(request_config.type == type_blog
       && !process_blog(rp, request_config, tpl, blogs, data))
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

bool data_handler::process_static(const request_parser& rp, const cache_storage& cache, const config& cfg, std::string& data)
{
    if(cfg.cache == cache_method::cache_static && cache.is_cached(cfg.location))
    {
        const std::string file_data = cache.get_static(cfg.location);
        data = response_200(file_data, cfg.mimetype);
        
        return true;
    }
    
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

bool data_handler::process_blog(const request_parser& rp, const config& cfg, const template_storage& tpl, const blog_storage& blogs, std::string& data)
{
    const auto& path = rp.get_path();
    const auto last_slash = path.find_last_of('/');
    if(last_slash == std::string::npos)
    {
        return false;
    }
    
    const auto id_str = path.substr(last_slash + 1, path.size());
    if(id_str.empty())
    {
        return false;
    }
    
    int id = 0;
    if(!utils::string_to_int(id_str, id))
    {
        return false;
    }
    
    std::string blog;
    std::string title;
    if(!blogs.get_blog(id, blog, title))
    {
        return false;
    }
    
    std::string blog_template;
    if(!tpl.get_template("blog", blog_template))
    {
        return false;
    }
    
    data = response_200(fmt::format(blog_template, title, blog), cfg.mimetype);

    return true;
}

bool data_handler::process_dynamic(const request_parser& rp, const config& cfg, const blog_storage& blogs, std::string& data)
{
    return false;
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
    
    return false;
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