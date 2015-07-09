#include "blog_storage.hpp"

#include "utils.hpp"

blog_storage::blog_storage(const std::string& blog_config)
{
    const auto blog_files = utils::file_to_array(blog_config);
    if(blog_files.size() == 0)
    {
        return;
    }
    
    for(const auto& blog_line : blog_files)
    {
        const auto blog_split = utils::split_string(blog_line, ' ', true);
        if(blog_split.size() < 4)
        {
            continue;
        }
        
        const int blog_id = std::stoi(blog_split[0]);
        const std::string blog_filename = blog_split[1];
        const std::string blog_date = blog_split[2];
        
        if(!utils::file_exists(blog_filename))
        {
            continue;
        }
        
        const std::string blog_data = utils::file_to_string(blog_filename);
        if(blog_data.size() == 0)
        {
            continue;
        }
        
        std::stringstream ss;
        for(int i = 3; i < blog_split.size(); ++i)
        {
           ss << blog_split[i] << ' ';
        }
        
        blog b;
        b.body = blog_data;
        b.date = blog_date;
        b.title = ss.str();
        b.id = blog_id;
        
        m_blogs.emplace_back(b);
    }
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