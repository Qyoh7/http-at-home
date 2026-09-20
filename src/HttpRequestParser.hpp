#include "HttpTypes.hpp"
#include <string>

namespace HttpRequestParser
{
    std::string recvRequest(int client_fd);
    Request parseRequest(std::string req);
}