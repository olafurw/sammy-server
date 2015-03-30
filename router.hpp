#include <functional>

namespace router
{
    void on_data(const int sck, const std::string& data);
    void run();
}