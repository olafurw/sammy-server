#include "template_storage.hpp"

#include "utils.hpp"

template_storage::template_storage(const std::string& template_config)
{
    const auto template_file = utils::file_to_array(template_config);
    if(template_file.size() == 0)
    {
        return;
    }
    
    for(const auto& tpl : template_file)
    {
        const auto tpl_split = utils::split_string(tpl, ' ', true);
        if(tpl_split.size() != 2)
        {
            continue;
        }
        
        const std::string& template_name = tpl_split[0];
        const std::string& template_path = tpl_split[1];
        
        if(!utils::file_exists(template_path))
        {
            continue;
        }
        
        const std::string template_data = utils::file_to_string(template_path);
        if(template_data.size() == 0)
        {
            continue;
        }
        
        m_templates[template_name] = template_data;
    }
}

bool template_storage::get_template(const std::string& name, std::string& tpl) const
{
    if(m_templates.count(name) == 0)
    {
        return false;
    }
    
    tpl = m_templates.at(name);
    
    return true;
}
