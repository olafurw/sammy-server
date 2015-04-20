#ifndef DATABASE_TYPES
#define DATABASE_TYPES

#include <soci/soci.h>
#include <iostream>

struct blog
{
    unsigned int id;
    std::string title;
    std::string body;
    unsigned int created_date;
    unsigned int modified_date;
    unsigned int votes;
};

namespace soci
{
template<> struct type_conversion<blog>
{
    typedef values base_type;
    static void from_base(values const& v, indicator /* ind */, blog& b)
    {
        b.id = v.get<unsigned int>("id");
        b.title = v.get<std::string>("title");
        b.body = v.get<std::string>("body");
        b.created_date = v.get<unsigned int>("created_date");
        b.modified_date = v.get<unsigned int>("modified_date");
        b.votes = v.get<unsigned int>("votes");
    }
    static void to_base(const blog& b, values& v, indicator& ind)
    {
        v.set("title", b.title);
        v.set("body", b.body);
        v.set("created_date", b.created_date);
        v.set("modified_date", b.modified_date);
        v.set("votes", b.votes);
        ind = i_ok;
    }
};
}

#endif
