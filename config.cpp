#include "config.hpp"

config_storage::config_storage(const std::string& config_file)
{
    const auto lines = utils::file_to_array(config_file);
    
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
        
        m_routes[line[1]] = c;
    }
}

bool config_storage::get(const std::string& key, config& cfg) const
{
    if(m_routes.count(key) == 0)
    {
        return false;
    }
    
    cfg = m_routes.at(key);
    
    return true;
}
