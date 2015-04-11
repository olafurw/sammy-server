#ifndef DATABASE
#define DATABASE

#include <soci/soci.h>

#include <string>
#include <memory>
#include <vector>

#include "database_types.hpp"

class database
{
public:
    database();
    
    bool insert_blog(const blog& b, int& id);
    bool get_blog(const int id, blog& b);

private:
    std::unique_ptr<soci::session> m_session;
};

#endif
