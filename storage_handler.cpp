#include "storage_handler.hpp"

storage_handler::storage_handler(const std::string& path)
    : logger(path)
    , blogs(path)
    , config(path, cache)
    , tpl(path)
{
}

void storage_handler::refresh()
{
    blogs.refresh();
    config.refresh(cache);
    tpl.refresh();
}
