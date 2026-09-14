#include "HttpTypes.hpp"
#include <vector>
#include <string>

namespace HttpRequestParser
{
    std::string recvRequest(int client_fd);
    std::vector<std::string> tokenize(std::string str);
    Request parseRequest(std::string req);
}