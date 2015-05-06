#include "blog_storage.hpp"

#include "utils.hpp"

blog_storage::blog_storage(const std::string& blog_config)
{
    m_highest_id = 0;
    
    const auto blog_files = utils::file_to_array(blog_config);
    if(blog_files.size() == 0)
    {
        return;
    }
    
    for(const auto& blog : blog_files)
    {
        const auto blog_split = utils::split_string(blog, ' ', true);
        if(blog_split.size() != 2)
        {
            continue;
        }
        
        const int blog_id = std::stoi(blog_split[0]);
        
        const std::string blog_filename = blog_split[1];
        if(!utils::file_exists(blog_filename))
        {
            continue;
        }
        
        const std::string blog_data = utils::file_to_string(blog_filename);
        if(blog_data.size() == 0)
        {
            continue;
        }
        
        m_blogs[blog_id] = blog_data;
        
        if(blog_id > m_highest_id)
        {
            m_highest_id = blog_id;
        }
    }
}

bool blog_storage::get_blog(const int id, std::string& blog) const
{
    if(m_blogs.count(id) == 0)
    {
        return false;
    }
    
    blog = m_blogs.at(id);
    
    return true;
}

bool blog_storage::get_latest_blog(std::string& blog) const
{
    return get_blog(m_highest_id, blog);
}