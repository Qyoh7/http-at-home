#include "HttpRequestHandler.hpp"
#include "HttpResponses.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include <cstring>
#include <magic_enum/magic_enum.hpp>
#include <stdexcept>
#include <string>
#include <sstream>
#include <sys/socket.h>

namespace HttpRequestHandler
{

    std::string responseToRawString(const Response& response)
    {
        std::stringstream raw_response;
        raw_response << response.statusLine << "\r\n";
        if (response.headers.has_value())
        {
            for (Header h : response.headers.value())
            {
                raw_response << Utils::headerTypeToString(h.type);
                raw_response << " " << h.value << "\r\n";
            }
        }
        raw_response << "\r\n";
        if (response.body.has_value())
        {
            raw_response << response.body.value();
        }
        Utils::log("Sent back: " + Utils::escapeStr(raw_response.str()), LogLevel::INFO);
        return raw_response.str();
    }

    void sendResponse(Response response, int client_fd)
    {
        std::string raw_response = responseToRawString(response);
        if (send(client_fd, raw_response.data(), raw_response.size(), 0) < 0)
        {
            throw std::runtime_error(std::string("Failed to send response: ") + std::strerror(errno));
        }
    }
    
    void handleRequest(const Request& request, int client_fd)
    {
        std::string target = Utils::stringToLower(request.target);
        Response response;
        switch (request.method)
        {
            case RequestMethod::GET:
                if (target.at(0) == '/' && target.size() == 1)
                {
                    response.statusLine = response200();
                    break;
                }
                else if (target.substr(0, 5) == "/echo")
                {
                    response.statusLine = response200();
                    response.body = request.target.substr(6, request.target.back());
                    break;
                }
                else if (target == "/user-agent")
                {
                    response.statusLine = response200();
                }
                else {
                    response.statusLine = response404();
                }
                break;
            default:
                throw std::runtime_error(std::string("Invalid HTTP Method: ") + magic_enum::enum_name(request.method).data());
                break;
        }
        sendResponse(response, client_fd);
    }
}
