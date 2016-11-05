#include "data_handler.hpp"

#include "libraries/json.hpp"
#include "libraries/format.h"

#include "utils.hpp"

data_handler::data_handler(const storage_handler* storage)
    : m_storage(storage)
{

}

void data_handler::process(const request_parser& rp, std::string& data)
{
    m_request = &rp;
    if(!m_storage->config.get(m_request->get_path(), m_request_config)
        || m_request_config.method != m_request->get_method())
    {
        data = response_error();
        return;
    }

    if(m_request_config.type == type_static
       && process_static(data))
    {
        return;
    }    
    else if(m_request_config.type == type_dynamic
       && process_dynamic(data))
    {
        return;
    }
    else if(m_request_config.type == type_blog
       && process_blog(data))
    {
        return;
    }
    else if(m_request_config.type == type_blog_list
       && process_blog_list(data))
    {
        return;
    }
    else if(m_request_config.type == type_template
       && process_template(data))
    {
        return;
    }
    else if(m_request_config.type == type_json
       && process_json(data))
    {
        return;
    }
    
    data = response_error();
}

bool data_handler::process_static(std::string& data)
{
    if(m_request_config.cache == cache_method::cache_static 
       && m_storage->cache.is_cached(m_request_config.path))
    {
        const std::string file_data = m_storage->cache.get_static(m_request_config.path);
        data = response_200(file_data, m_request_config.mimetype);
        
        return true;
    }
    
    if(!utils::file_exists(m_request_config.path))
    {
        return false;
    }
    
    if(m_request->is_data_requested())
    {
        const std::string file_data = utils::file_to_string(m_request_config.path);
        data = response_200(file_data, m_request_config.mimetype);
    }
    else
    {
        const int file_size = utils::file_size(m_request_config.path);
        data = response_200_head(file_size, m_request_config.mimetype);
    }
    
    return true;
}

bool data_handler::process_blog(std::string& data)
{
    const auto& path = m_request->get_path();
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
    
    blog b;
    if(!m_storage->blogs.get_blog(id, b))
    {
        return false;
    }
    
    std::string blog_entry_template;
    if(!m_storage->tpl.get_template("blog_entry", blog_entry_template))
    {
        return false;
    }
    
    std::string blog_template;
    if(!m_storage->tpl.get_template("blog", blog_template))
    {
        return false;
    }
    
    const std::string blog_entry = fmt::format(blog_entry_template, b.id, b.title, b.date, b.body);
    
    data = response_200(fmt::format(blog_template, blog_entry), m_request_config.mimetype);

    return true;
}

bool data_handler::process_blog_list(std::string& data)
{
    std::vector<blog> blogs;
    if(!m_storage->blogs.get_blogs(blogs)
        || blogs.empty())
    {
        return false;
    }
    
    std::string blog_entry_template;
    if(!m_storage->tpl.get_template("blog_entry", blog_entry_template))
    {
        return false;
    }
    
    std::string blog_template;
    if(!m_storage->tpl.get_template("blog", blog_template))
    {
        return false;
    }
    
    std::stringstream ss;
    
    for(const blog& b : blogs)
    {
        ss << fmt::format(blog_entry_template, b.id, b.title, b.date, b.body);
    }
    
    data = response_200(fmt::format(blog_template, ss.str()), m_request_config.mimetype);

    return true;
}

bool data_handler::process_template(std::string& data)
{
    std::string blog_template;
    if(!m_storage->tpl.get_template("blog", blog_template)
        || !utils::file_exists(m_request_config.path))
    {
        return false;
    }
    
    const std::string content = utils::file_to_string(m_request_config.path);
    
    data = response_200(fmt::format(blog_template, content), m_request_config.mimetype);

    return true;
}

bool data_handler::process_dynamic(std::string& data)
{
    return false;
}

bool data_handler::process_json(std::string& data)
{
    nlohmann::json request_json;
    try
    {
        request_json = nlohmann::json::parse(m_request->get_post_data());
    }
    catch(...)
    {
        return false;
    }
    
    return false;
}

std::string data_handler::response_error()
{
    static const std::string error = R"header(HTTP/1.0 404 Not Found
Server: sammy v0.5
MIME-version: 1.0
Content-type: text/plain
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: 28

Requested content not found!

)header";
    
    return error;
}

std::string data_handler::response_200(const std::string& input, const std::string& content_type)
{
    //Set-Cookie: LOL_SESSION=abc123; Expires=Wed, 16 Nov 2015 12:34:56 GMT
    static const std::string header = R"header(HTTP/1.0 200 OK
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
    static const std::string header = R"header(HTTP/1.0 200 OK
Server: sammy v0.5
MIME-version: 1.0
Content-type: {0}
Last-Modified: Thu, 1 Jan 1970 00:00:00 GMT
Content-Length: {1}

)header";
    
    return fmt::format(header, content_type, input_size);
}
