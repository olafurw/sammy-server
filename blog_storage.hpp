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
    std::vector<std::string> tags;
};

class blog_storage
{
public:
    blog_storage(const std::string& path);
    
    bool get_blogs(std::vector<blog>& blogs) const;
    bool get_blogs(std::vector<blog>& blogs, const int limit) const;
    bool get_blog(const int id, blog& out_blog) const;
    bool get_latest_blog(blog& out_blog) const;
    void refresh();
    
private:
    
    std::vector<blog> m_blogs;
    std::string m_path;
    std::string m_blog_config;
    
    std::string clean_tag(const std::string& tag);
    void load();
};

#endif
