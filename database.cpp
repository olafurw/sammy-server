#include "database.hpp"

#include <iostream>

database::database()
    : m_session("mysql", "db=cznp_database user=dummy password=dummy")
{
}

bool database::increment_button()
{
    m_session << "UPDATE button SET counter=counter+1 LIMIT 1;";
    
    return true;
}

bool database::get_button_counter(unsigned int& out_counter)
{
    soci::indicator ind;
    out_counter = 0;
    m_session << "SELECT counter FROM button LIMIT 1;", soci::into(out_counter, ind);
    
    return ind == soci::indicator::i_ok;
}