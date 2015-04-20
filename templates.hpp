#ifndef SERVER_TEMPLATES
#define SERVER_TEMPLATES

#include <map>
#include <string>

class templates
{
public:
    templates(const std::string& templates_file);
    
    bool get(const std::string& key, std::string& tpl) const;
private:
    std::map<std::string, std::string> m_templates;
};

#endif
