#ifndef BLOG_STORAGE
#define BLOG_STORAGE

#include <string>
#include <map>
#include <iostream>

class blog
{
public:
    std::string title;
    std::string body;
};

class blog_storage
{
public:
    blog_storage(const std::string& blog_config);
    
    bool get_blog(const int id, std::string& body, std::string& title) const;
    bool get_latest_blog(std::string& body, std::string& title) const;
private:
    
    std::map<int, blog> m_blogs;
    int m_highest_id;
};

#endif
