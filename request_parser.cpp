#include "request_parser.hpp"

#include "utils.hpp"

request_parser::request_parser(const int id, const std::string& request, const std::string& ip_address)
{
    m_error = 0;
    m_error_text = "";

    m_request = request;
    m_method = route_method::method_unknown;
    m_is_data_requested = false;
    m_path = "";
    m_host = "";
    m_if_modified_since = 0;
    m_port = 80;
    
    m_ip_address = ip_address;

    m_request_lines = utils::split_string(m_request, '\n');
    m_identifier = utils::sha256(std::to_string(id));

    if(m_request_lines.size() == 0 || m_request.empty())
    {
        m_error = 1;
        m_error_text = "Request data empty.";

        return;
    }

    parse_header();

    for(unsigned int i = 1; i < m_request_lines.size(); ++i)
    {
        // Find the line type of the request, to route it to the correct parse function
        const auto& current_line = m_request_lines.at(i);
        const auto colon_pos = current_line.find(": ");
        if(colon_pos == std::string::npos)
        {
            continue;
        }

        const std::string line_type = current_line.substr(0, colon_pos);
        const std::string line_data = current_line.substr(colon_pos + 2);

        if(line_type == "Host")
        {
            parse_host(line_data);

            continue;
        }

        if(line_type == "Cookie")
        {
            parse_cookies(line_data);

            continue;
        }

        if(line_type == "Referer")
        {
            parse_referer(line_data);

            continue;
        }

        if(line_type == "If-Modified-Since")
        {
            //parse_if_modified_since(line_data);

            continue;
        }
    }

    parse_post_data();

    if(m_method == route_method::method_unknown 
       || m_path.size() == 0 
       || m_host.size() == 0)
    {
        m_error = 1;
        m_error_text = "Method, path or host empty.";

        return;
    }
}

void request_parser::parse_header()
{
    std::vector<std::string> first_line = utils::split_string(m_request_lines.at(0), ' ');

    if(first_line.size() < 3)
    {
        return;
    }

    std::string& method = first_line.at(0);

    if(method == "GET")
    {
        m_method = route_method::method_get;
        m_is_data_requested = true;
    }
    else if(method == "HEAD")
    {
        m_method = route_method::method_get;
    }
    else if(method == "POST")
    {
        m_method = route_method::method_post;
        m_is_data_requested = true;
    }

    m_path = utils::trim(first_line.at(1));

    return;
}

void request_parser::parse_host(const std::string& host_data)
{
    auto host = utils::split_string(host_data, ':');

    if(host.size() == 0)
    {
        m_error = 1;
        m_error_text = "Host information empty.";
    }

    m_host = utils::trim(host[0]);

    if(host.size() > 1)
    {
        try
        {
            m_port = std::stoul(host[1]);
        }
        catch(...)
        {
            m_error = 1;
            m_error_text = "Error parsing port value:" + host[1];
        }
    }
}

void request_parser::parse_cookies(const std::string& cookie_data)
{
    auto cookies = utils::split_string(cookie_data, ';');

    for(auto& cookie : cookies)
    {
        auto eq_pos = cookie.find("=");
        std::string key = utils::trim(cookie.substr(0, eq_pos));
        std::string value = utils::trim(cookie.substr(eq_pos + 1));

        if(key == "" || value == "")
        {
            continue;
        }

        m_cookies[key] = value;
    }
}

void request_parser::parse_post_data()
{
    if(m_method != route_method::method_post)
    {
        return;
    }

    std::stringstream post_stream;

    bool found_empty_line = false;
    for(const auto& line : m_request_lines)
    {
        if(!found_empty_line && utils::trim(line) == "")
        {
            found_empty_line = true;
        }

        if(found_empty_line)
        {
            post_stream << line;
        }
    }

    m_post_data = utils::trim(post_stream.str());
}

void request_parser::parse_referer(const std::string& referer_data)
{
    m_referer = utils::trim(referer_data);
}

void request_parser::parse_date_rfc_1123(const std::vector<std::string>& parts)
{
    const auto& wkday = parts[0];
    const int day = std::stoi(parts[1], 0, 10);
    if(day < 0 || day > 31)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since day field not correct. Day: " + std::to_string(day);

        return;
    }

    int month = -1;
    const auto& month_text = parts[2];
    if(month_text == "Jan")
    {
        month = 0;
    }
    else if(month_text == "Feb")
    {
        month = 1;
    }
    else if(month_text == "Mar")
    {
        month = 2;
    }
    else if(month_text == "Apr")
    {
        month = 3;
    }
    else if(month_text == "May")
    {
        month = 4;
    }
    else if(month_text == "Jun")
    {
        month = 5;
    }
    else if(month_text == "Jul")
    {
        month = 6;
    }
    else if(month_text == "Aug")
    {
        month = 7;
    }
    else if(month_text == "Sep")
    {
        month = 8;
    }
    else if(month_text == "Oct")
    {
        month = 9;
    }
    else if(month_text == "Nov")
    {
        month = 10;
    }
    else if(month_text == "Dec")
    {
        month = 11;
    }

    if(month == -1)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since month field not correct. Month: " + month_text;

        return;
    }

    const int year = std::stoi(parts[3], 0, 10);
    const std::string year_test = std::to_string(year);
    if(parts[3] != year_test)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since year parsing not successful.";

        return;
    }

    if(year < 0 || (year - 1900) < 0)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since year field not large enough.";

        return;
    }

    const auto& time_text = parts[4];
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    const int scan_result = sscanf(time_text.c_str(), "%2d:%2d:%2d", &hours, &minutes, &seconds);
    if(scan_result != 3)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since time field not correct.";
    }

    if(hours < 0 || hours > 23)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since hours field not correct.";
    }

    if(minutes < 0 || minutes > 59)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since minutes field not correct.";
    }

    if(seconds < 0 || seconds > 60)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since seconds field not correct.";
    }

    const auto& gmt = parts[5];
    if(gmt != "GMT")
    {
        m_error = 1;
        m_error_text = "If-Modified-Since GMT missing.";
    }

    tm* time_object = new tm;
    time_object->tm_sec = seconds;
    time_object->tm_min = minutes;
    time_object->tm_hour = hours;
    time_object->tm_mday = day;
    time_object->tm_mon = month;
    time_object->tm_year = year - 1900;

    const time_t result = timegm(time_object);
    if(result == -1)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since unable to generate date.";

        return;
    }

    // Success
    m_if_modified_since = result;
}

// Not implemented yet
void request_parser::parse_date_rfc_850(const std::vector<std::string>& parts)
{
    m_if_modified_since = 0;
}

// Not implemented yet
void request_parser::parse_date_asctime(const std::vector<std::string>& parts)
{
    auto wkday = parts[0];

    int month = -1;
    auto month_text = parts[1];
    if(month_text == "Jan")
    {
        month = 0;
    }
    else if(month_text == "Feb")
    {
        month = 1;
    }
    else if(month_text == "Mar")
    {
        month = 2;
    }
    else if(month_text == "Apr")
    {
        month = 3;
    }
    else if(month_text == "May")
    {
        month = 4;
    }
    else if(month_text == "Jun")
    {
        month = 5;
    }
    else if(month_text == "Jul")
    {
        month = 6;
    }
    else if(month_text == "Aug")
    {
        month = 7;
    }
    else if(month_text == "Sep")
    {
        month = 8;
    }
    else if(month_text == "Oct")
    {
        month = 9;
    }
    else if(month_text == "Nov")
    {
        month = 10;
    }
    else if(month_text == "Dec")
    {
        month = 11;
    }

    if(month == -1)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since month field not correct. Month: " + month_text;

        return;
    }

    int day = std::stoi(parts[2], 0, 10);
    if(day < 0 || day > 31)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since day field not correct.";

        return;
    }

    auto time_text = parts[3];
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    int scan_result = sscanf(time_text.c_str(), "%2d:%2d:%2d", &hours, &minutes, &seconds);
    if(scan_result != 3)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since time field not correct.";
    }

    if(hours < 0 || hours > 23)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since hours field not correct.";
    }

    if(minutes < 0 || minutes > 59)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since minutes field not correct.";
    }

    if(seconds < 0 || seconds > 60)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since seconds field not correct.";
    }

    int year = std::stoi(parts[4], 0, 10);
    std::string year_test = std::to_string(year);
    if(parts[4] != year_test)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since year parsing not successful.";

        return;
    }

    if(year < 0 || (year - 1900) < 0)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since year field not large enough.";

        return;
    }

    tm* time_object = new tm;
    time_object->tm_sec = seconds;
    time_object->tm_min = minutes;
    time_object->tm_hour = hours;
    time_object->tm_mday = day;
    time_object->tm_mon = month;
    time_object->tm_year = year - 1900;

    time_t result = timegm(time_object);
    if(result == -1)
    {
        m_error = 1;
        m_error_text = "If-Modified-Since unable to generate date.";

        return;
    }

    // Success
    m_if_modified_since = result;
}

void request_parser::parse_if_modified_since(const std::string& modified_date)
{
    auto parts = utils::split_string(modified_date, ' ', true);

    // RFC 1123 or RFC 850
    if(parts.size() == 6 && parts[5] == "GMT")
    {
        // RFC 1123
        if(parts[0].length() == 4)
        {
            parse_date_rfc_1123(parts);
            return;
        }

        parse_date_rfc_850(parts);
        return;
    }

    parse_date_asctime(parts);
    return;
}

std::string request_parser::get_host() const
{
    return m_host;
}

std::string request_parser::get_path() const
{
    return m_path;
}

route_method request_parser::get_method() const
{
    return m_method;
}

bool request_parser::is_data_requested() const
{
    return m_is_data_requested;
}

std::string request_parser::get_post_data() const
{
    return m_post_data;
}

std::string request_parser::get_referer() const
{
    return m_referer;
}

std::string request_parser::get_identifier() const
{
    return m_identifier;
}

unsigned int request_parser::get_port() const
{
    return m_port;
}

time_t request_parser::get_if_modified_since() const
{
    return m_if_modified_since;
}

bool request_parser::errors() const
{
    return m_error != 0;
}

std::string request_parser::error_text() const
{
    return m_error_text;
}

std::string request_parser::to_string() const
{
    nlohmann::json j;
    
    j["route_method"] = "unknown";
    if(m_method == route_method::method_get)
    {
        j["route_method"] = "get";
    }
    else if(m_method == route_method::method_post)
    {
        j["route_method"] = "post";
    }
    
    j["path"] = m_path;
    j["host"] = m_host;
    j["port"] = m_port;
    j["id"] = m_identifier;
    j["referer"] = m_referer;
    j["data_requested"] = m_is_data_requested;
    j["ip_address"] = m_ip_address;
    
    if(errors())
    {
        j["error"] = m_error_text;
    }
    
    j["raw"] = m_request;
    
    return j.dump();
}
