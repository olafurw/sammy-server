#ifndef SERV_REQUEST_PARSER
#define SERV_REQUEST_PARSER

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "config.hpp"

class request_parser
{
public:
    request_parser(const int id, const std::string& request);

    std::string get_host() const;
    std::string get_path() const;
    route_method get_method() const;
    bool is_data_requested() const;
    std::string get_identifier() const;
    std::string get_post_data() const;
    std::string get_referer() const;
    unsigned int get_port() const;
    time_t get_if_modified_since() const;

    bool errors() const;
    std::string error_text() const;

private:
    void parse_header();
    void parse_host(const std::string& host_data);
    void parse_cookies(const std::string& cookie_data);
    void parse_post_data();
    void parse_referer(const std::string& referer_data);

    void parse_date_rfc_1123(const std::vector<std::string>& parts);
    void parse_date_rfc_850(const std::vector<std::string>& parts);
    void parse_date_asctime(const std::vector<std::string>& parts);

    void parse_if_modified_since(const std::string& modified_date);

    route_method m_method;
    bool m_is_data_requested;
    std::string m_path;
    std::string m_host;
    std::string m_identifier;
    std::string m_post_data;
    std::string m_referer;
    unsigned int m_port;
    time_t m_if_modified_since;
    std::map<std::string, std::string> m_cookies;

    int m_error;
    std::string m_error_text;

    std::vector<std::string> m_request_lines;
};

#endif
