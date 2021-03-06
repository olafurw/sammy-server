#ifndef ROUT_CONFIG
#define ROUT_CONFIG

#include <map>
#include <string>
#include <iostream>

#include "cache_storage.hpp"

enum route_type
{
    type_unknown,
    type_static,
    type_dynamic,
    type_blog,
    type_blog_list,
    type_template,
    type_json,
};

enum route_method
{
    method_unknown,
    method_get,
    method_post,
};

enum cache_method
{
    cache_never,
    cache_static,
};

class config
{
public:
    route_method method;
    std::string path;
    std::string location;
    route_type type;
    cache_method cache;
    std::string mimetype;
};

class config_storage
{
public:
    config_storage(const std::string& path, cache_storage& cache);
    
    bool get(const std::string& key, config& cfg) const;
    void refresh(cache_storage& cache);
    
private:
    std::map<std::string, config> m_routes;
    std::string m_path;
    std::string m_config_file;
    
    void load(cache_storage& cache);
};

#endif
