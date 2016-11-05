#ifndef REQUEST_LOGGER
#define REQUEST_LOGGER

#include <string>
#include <fstream>

class request_logger
{
public:
    request_logger(const std::string& directory);
    ~request_logger();
    void log(const std::string& data);
    
private:
    
    std::ofstream m_file;
    std::string m_directory;
};

#endif
