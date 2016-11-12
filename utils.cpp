#include "utils.hpp"

#include "libraries/sha256.hpp"

namespace utils
{
    
std::string sha256(const std::string& data)
{
    static char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned char result[32];
    sha256_state md;

    sha_init(md);
    sha_process(md, data.c_str(), data.size());
    sha_done(md, result);

    std::string sha_string;
    for(int i = 0; i < 32; ++i)
    {
        sha_string.append(&hex[(result[i] & 0xF0) >> 4], 1);
        sha_string.append(&hex[result[i] & 0xF], 1);
    }

    return sha_string;
}

time_t file_modified(const std::string& filename)
{
    struct stat sb;
    const int result = stat(filename.c_str(), &sb);
    
    if(result == -1)
    {
        return -1;
    }
    
    return sb.st_mtime;
}

std::string current_time(const char* format)
{
    time_t rawtime = current_time();
    tm* timeinfo = localtime(&rawtime);

    char buffer[80];
    strftime(buffer, 80, format, timeinfo);
    
    return std::string(buffer);
}

time_t current_time()
{
    time_t rawtime;
    time(&rawtime);
    
    return rawtime;
}

bool string_to_int(const std::string& value, int& out_value)
{
    int id = 0;
    try
    {
        id = stoi(value);
        
        const std::string tmp_check = std::to_string(id);
        if(tmp_check != value)
        {
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
    
    out_value = id;
    
    return true;
}

std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string& rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

std::string ltrim(const std::string& s)
{
    std::string tmp_str = s;
    tmp_str.erase(tmp_str.begin(), std::find_if(tmp_str.begin(), tmp_str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return tmp_str;
}

std::string rtrim(const std::string& s)
{
    std::string tmp_str = s;
    tmp_str.erase(std::find_if(tmp_str.rbegin(), tmp_str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), tmp_str.end());
    return tmp_str;
}

std::string trim(const std::string& s)
{
    std::string tmp_str = s;
    return ltrim(rtrim(tmp_str));
}

std::vector<std::string> split_string(const std::string& str, const char token, const bool skip_empty)
{
    std::vector<std::string> segments;
    std::stringstream ss(str);
    std::string segment;

    while(std::getline(ss, segment, token))
    {
        if(skip_empty && trim(segment) == "")
        {
            continue;
        }
        segments.emplace_back(std::move(segment));
    }

    return segments;
}

void replace(std::string& s, const std::string& f, const std::string& r)
{
    for(std::string::size_type i = 0; (i = s.find(f, i)) != std::string::npos;)
    {
        s.replace(i, f.length(), r);
        i += r.length();
    }
}

bool starts_with(const std::string& key, const std::string& target)
{
    const int key_length = key.size();
    const int target_length = target.size();
    if(key_length > target_length)
    {
        return false;
    }
    
    return strncmp(key.c_str(), target.c_str(), key_length) == 0;
}

bool file_exists(const std::string& filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    
    return in.good();
}

std::string file_to_string(const std::string& filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);

    std::string content;

    in.seekg(0, std::ios::end);
    content.resize(in.tellg());
    in.seekg(0, std::ios::beg);

    in.read(&content[0], content.size());
    in.close();

    return content;
}

std::size_t file_size(const std::string& filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);

    std::string content;

    in.seekg(0, std::ios::end);
    content.resize(in.tellg());
    in.close();

    return content.size();
}

std::vector<std::string> file_to_array(const std::string& filename, char token)
{
    return split_string(file_to_string(filename), token);
}

void write_to_file(const std::string& filename, const std::string& content)
{
    std::ofstream out(filename, std::ios::out | std::ios::app);
    out << content;
    out.close();
}

std::string current_working_directory()
{
    static const int cwd_size = 1024;
    char cwd[cwd_size];

    if(getcwd(cwd, cwd_size) == nullptr)
    {
        return "";
    }

    return std::string(cwd);
}

}
