#ifndef CACHE_STORAGE
#define CACHE_STORAGE

#include <map>

class cache_storage
{
public:
    cache_storage();
    
    bool add_static(const std::string& location);
    bool is_cached(const std::string& location) const;
    std::string get_static(const std::string& location) const;
    
private:
    std::map<std::string, std::string> m_cache;
};

#endif
