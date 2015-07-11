#ifndef TEMPLATE_STORAGE
#define TEMPLATE_STORAGE

#include <string>
#include <map>
#include <iostream>

class template_storage
{
public:
    template_storage(const std::string& template_config);
    void refresh();
    
    bool get_template(const std::string& name, std::string& tpl) const;
private:
    
    std::map<std::string, std::string> m_templates;
    std::string m_template_config;
    
    void load();
};

#endif
