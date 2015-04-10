#ifndef DATABASE_TYPES
#define DATABASE_TYPES

#include <soci/soci.h>

class id;
struct blog
{
    int id;
    std::string title;
    std::string body;
    int created_date;
    int modified_date;
    int votes;
};

namespace soci
{
template<> struct type_conversion<blog>
{
    typedef values base_type;
    static void from_base(values const& v, indicator /* ind */, blog& b)
    {
        b.id = v.get<int>("id");
        b.title = v.get<std::string>("title");
        b.body = v.get<std::string>("body");
        b.created_date = v.get<int>("created_date");
        b.modified_date = v.get<int>("modified_date");
        b.votes = v.get<int>("votes");
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
