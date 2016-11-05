#include "cache_storage.hpp"

#include "utils.hpp"

cache_storage::cache_storage()
{

}

bool cache_storage::add_static(const std::string& location)
{
    if(is_cached(location)
        || !utils::file_exists(location)
    )
    {
        return false;
    }
    
    m_cache[location] = utils::file_to_string(location);
    
    return true;
}

bool cache_storage::is_cached(const std::string& location) const
{
    return m_cache.find(location) != m_cache.end();
}

std::string cache_storage::get_static(const std::string& location) const
{
    if(!is_cached(location))   
    {
        return "";
    }
    
    return m_cache.at(location);
}

void cache_storage::clear()
{
    m_cache.clear();
}
