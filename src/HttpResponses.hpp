#include <string>

inline std::string response200()
{
    return "HTTP/1.1 200 OK\r\n";
}
inline std::string response404()
{
    return "HTTP/1.1 404 Not Found\r\n";
}