#include "HttpRequestHandler.hpp"
#include "HttpResponses.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include "magic_enum/magic_enum.hpp"
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace HttpRequestHandler
{
    void handleRequest(const Request& request, int client_fd)
    {
        switch (request.method)
        {
            case RequestMethod::GET:
                if (Utils::stringToLower(request.target.substr(0, 6)) == "/echo/")
                {
                    std::string response = response200();
                    response += request.target.substr(6, request.target.back());
                    send(client_fd, response.data(), response.size(), 0);
                    break;
                }
                break;
            default:
                throw std::runtime_error(std::string("Invalid HTTP Method: ") + magic_enum::enum_name(request.method).data());
                break;
        }
    }
}
