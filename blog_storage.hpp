#ifndef BLOG_STORAGE
#define BLOG_STORAGE

#include <string>
#include <map>
#include <iostream>
#include <vector>

class blog
{
public:
    int id;
    std::string title;
    std::string date;
    std::string body;
};

class blog_storage
{
public:
    blog_storage(const std::string& blog_config);
    
    bool get_blogs(std::vector<blog>& blogs) const;
    bool get_blogs(std::vector<blog>& blogs, const int limit) const;
    bool get_blog(const int id, blog& out_blog) const;
    bool get_latest_blog(blog& out_blog) const;
private:
    
    std::vector<blog> m_blogs;
};

#endif
