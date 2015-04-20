#include "templates.hpp"

#include "utils.hpp"

templates::templates(const std::string& templates_file)
{
    const auto lines = utils::file_to_array(templates_file);
    
    for(const auto& tpl : lines)
    {
        const auto line = utils::split_string(tpl, ' ');
        if(line.size() != 2)
        {
            continue;
        }
        
        const std::string& filename = line[1];
        if(!utils::file_exists(filename))
        {
            continue;
        }
        
        m_templates[line[0]] = utils::file_to_string(filename);
    }
}

bool templates::get(const std::string& key, std::string& tpl) const
{
    if(m_templates.count(key) == 0)
    {
        return false;
    }
    
    tpl = m_templates.at(key);
    
    return true;
}
