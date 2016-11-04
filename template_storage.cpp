#include "template_storage.hpp"

#include "utils.hpp"

template_storage::template_storage(const std::string& path)
    : m_path(path)
    , m_template_config(path + "templates.cfg")
{
    load();
}

void template_storage::load()
{
    const auto template_string = utils::file_to_string(m_template_config);
    if(template_string.empty())
    {
        return;
    }
    
    nlohmann::json template_json;
    try
    {
        template_json = nlohmann::json::parse(template_string);
    }
    catch(...)
    {
        return;
    }
    
    for(const auto template_entry : template_json)
    {        
        const std::string& template_name = template_entry["name"];
        const std::string& template_path = template_entry["path"];
        
        if(!utils::file_exists(m_path + template_path))
        {
            continue;
        }
        
        const std::string template_data = utils::file_to_string(m_path + template_path);
        if(template_data.size() == 0)
        {
            continue;
        }
        
        m_templates[template_name] = template_data;
    }
}

void template_storage::refresh()
{
    m_templates.clear();
    load();
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
