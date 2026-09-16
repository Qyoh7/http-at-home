#include <string>

inline std::string response200()
{
    return "HTTP/1.1 200 OK";
}
inline std::string response404()
{
    return "HTTP/1.1 404 Not Found";
}
inline std::string response201()
{
    return "HTTP/1.1 201 Created";
}
