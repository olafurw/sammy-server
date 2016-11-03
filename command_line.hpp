#ifndef COMMAND_LINE
#define COMMAND_LINE

#include <string>
#include <map>

class command_line
{
public:
    command_line(const int argc, char** argv);
    
    bool has_error() const;
    bool has_argument_key(const std::string& argument_key) const;
    bool has_argument_value(const std::string& argument_key) const;
    std::string get_argument_value(const std::string& argument_key) const;
    
private:
    bool m_error;
    std::map<std::string, std::string> m_arguments;
    
    bool is_argument_key(const std::string& argument) const;
};

#endif

