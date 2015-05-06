#ifndef BLOG_STORAGE
#define BLOG_STORAGE

#include <string>
#include <map>
#include <iostream>

class blog_storage
{
public:
    blog_storage(const std::string& blog_config);
    
    bool get_blog(const int id, std::string& blog) const;
    bool get_latest_blog(std::string& blog) const;
private:
    
    std::map<int, std::string> m_blogs;
    int m_highest_id;
};

#endif
