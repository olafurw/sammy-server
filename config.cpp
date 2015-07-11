#include "config.hpp"

#include "utils.hpp"

config_storage::config_storage(const std::string& config_file, cache_storage& cache)
    : m_config_file(config_file)
{
    load(cache);
}

void config_storage::load(cache_storage& cache)
{
    cache.clear();
    
    const auto lines = utils::file_to_array(m_config_file);
    
    for(const auto& route : lines)
    {
        const auto line = utils::split_string(route, ' ');

        config c;
        c.location = line[2];
        c.path = line[1];
        
        c.type = type_unknown;
        if(line[3] == "STATIC")
        {
            c.type = type_static;
        }
        else if(line[3] == "JSON")
        {
            c.type = type_json;
        }
        else if(line[3] == "DYNAMIC")
        {
            c.type = type_dynamic;
        }
        else if(line[3] == "BLOG")
        {
            c.type = type_blog;
        }
        else if(line[3] == "BLOG_LIST")
        {
            c.type = type_blog_list;
        }
        
        c.method = method_unknown;
        if(line[0] == "GET")
        {
            c.method = method_get;
        }
        else if(line[0] == "POST")
        {
            c.method = method_post;
        }
        
        c.mimetype = line[4];
        
        c.cache = cache_never;
        if(line[5] == "CACHE-STATIC")
        {
            c.cache = cache_static;
            
            if(!cache.add_static(c.location))
            {
                std::cout << c.path << " not added to cache or config" << std::endl;
                
                continue;
            }
        }
        
        m_routes[line[1]] = c;
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

