#include "request_logger.hpp"
#include "utils.hpp"

request_logger::request_logger(const std::string& directory)
    : m_directory(directory + "logs/")
    , m_file(directory + "logs/" + "server.log", std::ios::out | std::ios::app)
{
}

request_logger::~request_logger()
{
    m_file.close();
}

void request_logger::log(const std::string& data)
{
    m_file << data << "\n";
    m_file.flush();
}
