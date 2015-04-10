#include "database.hpp"
#include "utils.hpp"

#include <iostream>

database::database()
    : m_session("mysql", "db=cznp_database user=dummy password=dummy")
{
}

bool database::insert_blog(const blog& b, int& id)
{
    m_session << "INSERT INTO blog (title, body, created_date, modified_date, votes) VALUES (:title, :body, :created_date, :modified_date, :votes)",
        soci::use(b);
    
    id = 0;
    m_session << "SELECT LAST_INSERT_ID();", soci::into(id);
    
    return true;
}

bool database::get_blog(const int id, blog& b)
{
    m_session << "SELECT id, body, created_date, modified_date, votes WHERE id = :id LIMIT 1;", 
        soci::use(id), soci::into(b);
    
    return true;
}