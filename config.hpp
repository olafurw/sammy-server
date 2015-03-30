#ifndef ROUT_CONFIG
#define ROUT_CONFIG

#include <map>
#include <string>
#include <iostream>

#include "utils.hpp"

enum route_type
{
    type_unknown,
    type_static,
    type_static_folder,
};

enum route_method
{
    method_unknown,
    method_get,
    method_post,
};

class config
{
public:
    route_method method;
    std::string path;
    std::string location;
    route_type type;
    std::string mimetype;
};

class config_storage
{
public:
    config_storage(const std::string& config_file);
    
    bool get(const std::string& key, config& cfg) const;
    
private:
    std::map<std::string, config> m_routes;
};

#endif
