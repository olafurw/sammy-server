#ifndef DATABASE
#define DATABASE

#include <soci/soci.h>

#include <string>
#include <vector>

class database
{
public:
    database();
    
    bool increment_button();
    bool get_button_counter(unsigned int& out_counter);

private:
    soci::session m_session;
};

#endif
