#include "config.hpp"

#include "utils.hpp"

config_storage::config_storage(const std::string& path, cache_storage& cache)
    : m_path(path)
    , m_config_file(path + "config.cfg")
{
    load(cache);
}

void config_storage::load(cache_storage& cache)
{
    cache.clear();
    
    const auto config_string = utils::file_to_string(m_config_file);
    if(config_string.empty())
    {
        return;
    }
    
    nlohmann::json config_json;
    try
    {
        config_json = nlohmann::json::parse(config_string);
    }
    catch(...)
    {
        return;
    }
    
    for(const auto config_entry : config_json)
    {
        config c;
        c.location = config_entry["route"].get<std::string>();
        c.path = m_path + config_entry["path"].get<std::string>();
        
        const std::string type = config_entry["type"];
        
        c.type = type_unknown;
        if(type == "STATIC")
        {
            c.type = type_static;
        }
        else if(type == "JSON")
        {
            c.type = type_json;
        }
        else if(type == "DYNAMIC")
        {
            c.type = type_dynamic;
        }
        else if(type == "BLOG")
        {
            c.type = type_blog;
        }
        else if(type == "BLOG_LIST")
        {
            c.type = type_blog_list;
        }
        else if(type == "TEMPLATE")
        {
            c.type = type_template;
        }
        
        const std::string method = config_entry["method"];
        
        c.method = method_unknown;
        if(method == "GET")
        {
            c.method = method_get;
        }
        else if(method == "POST")
        {
            c.method = method_post;
        }
        
        c.mimetype = config_entry["mimetype"].get<std::string>();
        
        const std::string cache_type = config_entry["cache"];
        
        c.cache = cache_never;
        if(cache_type == "CACHE-STATIC")
        {
            c.cache = cache_static;
            
            if(!cache.add_static(c.path))
            {
                std::cout << c.path << " not added to cache or config" << std::endl;
                
                continue;
            }
        }
        
        m_routes[c.location] = c;
    }
}

void config_storage::refresh(cache_storage& cache)
{
    m_routes.clear();
    load(cache);
}

bool config_storage::get(const std::string& key, config& cfg) const
{
    if(m_routes.count(key) == 0)
    {
        for(const auto& route : m_routes)
        {
            const std::string& route_key = route.first;
            const config& c = route.second;
            if(c.type != type_dynamic && c.type != type_blog)
            {
                continue;
            }
            
            if(key.compare(0, route_key.size(), route_key) == 0)
            {
                cfg = c;

                return true;
            }
        }
        
        return false;
    }
    
    cfg = m_routes.at(key);
    
    return true;
}

