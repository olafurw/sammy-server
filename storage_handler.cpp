#include "storage_handler.hpp"

storage_handler::storage_handler(const std::string& path)
    : logger(path + "logs/"), blogs(path + "blog.cfg"), config(path + "config.cfg", cache), tpl(path + "templates.cfg")
{
}
