#ifndef STORAGE_HANDLER
#define STORAGE_HANDLER

#include "request_logger.hpp"
#include "cache_storage.hpp"
#include "blog_storage.hpp"
#include "template_storage.hpp"
#include "config.hpp"

class storage_handler
{
public:
    storage_handler(const std::string& path);
    void refresh();
    
    request_logger logger;
    cache_storage cache;
    blog_storage blogs;
    config_storage config;
    template_storage tpl;
};

#endif