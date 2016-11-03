#include "command_line.hpp"

#include <iostream>

command_line::command_line(const int argc, char** argv)
    : m_error(false)
{
    if(argc < 2)
    {
        return;
    }
    
    for(int i = 1; i < argc;)
    {
        const std::string key = argv[i];
        if(!is_argument_key(key))
        {
            m_error = true;
            return;
        }
        
        m_arguments[key] = "";
        
        if(i + 1 >= argc)
        {
            return;
        }
        
        const std::string value = argv[i + 1];
        if(is_argument_key(value))
        {
            ++i;
            continue;
        }
        
        m_arguments[key] = value;
        i += 2;
    }
}

bool command_line::has_error() const
{
    return m_error;
}

bool command_line::has_argument_key(const std::string& argument_key) const
{
    if(has_error())
    {
        return false;
    }
    
    return m_arguments.count(argument_key) > 0;
}

bool command_line::has_argument_value(const std::string& argument_key) const
{
    return !get_argument_value(argument_key).empty();
}

std::string command_line::get_argument_value(const std::string& argument_key) const
{
    if(has_error() || !has_argument_key(argument_key))
    {
        return "";
    }
    
    return m_arguments.at(argument_key);
}

bool command_line::is_argument_key(const std::string& argument) const
{
    return !argument.empty() && argument[0] == '-';
}
