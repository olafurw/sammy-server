#include "blog_storage.hpp"

#include "utils.hpp"

blog_storage::blog_storage(const std::string& blog_config)
    : m_blog_config(blog_config)
{
    load();
}

void blog_storage::load()
{
    const auto blog_config = utils::file_to_string(m_blog_config);
    if(blog_config.empty())
    {
        return;
    }
    
    nlohmann::json blog_json;
    try
    {
        blog_json = nlohmann::json::parse(blog_config);
    }
    catch(...)
    {
        return;
    }
    
    for(const auto blog_entry : blog_json)
    {
        const int blog_id = blog_entry["id"];
        const std::string blog_title = blog_entry["title"];
        const std::string blog_date = blog_entry["date"];
        const std::string blog_path = blog_entry["path"];
        if(!utils::file_exists(blog_path))
        {
            continue;
        }
        
        const std::string blog_data = utils::file_to_string(blog_path);
        if(blog_data.empty())
        {
            continue;
        }
        
        blog b;
        b.body = blog_data;
        b.date = blog_date;
        b.title = blog_title;
        b.id = blog_id;
        
        m_blogs.emplace_back(b);
    }
}

void blog_storage::refresh()
{
    m_blogs.clear();
    load();
}

bool blog_storage::get_blogs(std::vector<blog>& blogs) const
{
    if(m_blogs.empty())
    {
        return false;
    }
    
    for(const blog& b : m_blogs)
    {
        blogs.push_back(b);
    }
    
    return true;
}

bool blog_storage::get_blogs(std::vector<blog>& blogs, const int limit) const
{
    if(m_blogs.empty() || limit <= 0)
    {
        return false;
    }
    
    int counter = 0;
    
    for(const blog& b : m_blogs)
    {
        blogs.push_back(b);
        
        ++counter;
        if(counter == limit)
        {
            break;
        }
    }
    
    return true;
}

bool blog_storage::get_blog(const int id, blog& out_blog) const
{
    if(m_blogs.empty() || id <= 0)
    {
        return false;
    }
    
    for(const blog& b : m_blogs)
    {
        if(b.id == id)
        {
            out_blog = b;
            return true;
        }
    }
    
    return false;
}

bool blog_storage::get_latest_blog(blog& out_blog) const
{
    if(m_blogs.empty())
    {
        return false;
    }
    
    out_blog = m_blogs.back();
    
    return true;
}