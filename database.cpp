#include "database.hpp"
#include "utils.hpp"

#include <iostream>

database::database()
{
    const auto db_config = utils::file_to_array("/home/cznp-server/database.cfg");
    if(db_config.size() != 3)
    {
        std::cout << "Error in the database config" << std::endl;
        return;
    }
    
    std::string session_info = fmt::format("db={0} user={1} password={2}", db_config[0], db_config[1], db_config[2]);
    m_session = std::unique_ptr<soci::session>(new soci::session("mysql", session_info));
}

bool database::insert_blog(const blog& b, int& id)
{
    (*m_session) << "INSERT INTO blog (title, body, created_date, modified_date, votes) VALUES (:title, :body, :created_date, :modified_date, :votes)",
        soci::use(b);
    
    id = 0;
    (*m_session) << "SELECT LAST_INSERT_ID();", soci::into(id);
    
    return true;
}

bool database::get_blog(const int id, blog& b)
{
    (*m_session) << "SELECT id, body, created_date, modified_date, votes WHERE id = :id LIMIT 1;", 
        soci::use(id), soci::into(b);
    
    return true;
}