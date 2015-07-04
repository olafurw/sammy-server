#ifndef TEMPLATE_STORAGE
#define TEMPLATE_STORAGE

#include <string>
#include <map>
#include <iostream>

class template_storage
{
public:
    template_storage(const std::string& template_config);
    
    bool get_template(const std::string& name, std::string& tpl) const;
private:
    
    std::map<std::string, std::string> m_templates;
};

#endif
