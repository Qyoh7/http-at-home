#include "HttpRequestHandler.hpp"
#include "HttpResponses.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include <filesystem>
#include <cstring>
#include <magic_enum/magic_enum.hpp>
#include <stdexcept>
#include <string>
#include <sstream>
#include <sys/socket.h>

namespace HttpRequestHandler
{
    std::string headerTypeToString(const HeaderType& type)
    {
        switch (type)
        {
            case HeaderType::HOST:
                return "Host:";
            case HeaderType::CONTENT_TYPE:
                return "Content-Type";
            case HeaderType::CONTENT_LENGTH:
                return "Content-Length";
            case HeaderType::ACCEPT:
                return "Accept:";
            case HeaderType::USER_AGENT:
                return "User-Agent";
            default:
                throw std::logic_error(std::string("Invalid header type when converting to string: ") + std::string(magic_enum::enum_name(type)));
        }
    }

    std::string responseToRawString(const Response& response)
    {
        std::stringstream raw_response;
        raw_response << response.statusLine << "\r\n";
        if (response.headers.has_value())
        {
            for (auto it = response.headers.value().begin(); it != response.headers.value().end(); it++)
            {
                raw_response << headerTypeToString(it->first);
                raw_response << " " << it->second<< "\r\n";
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
                    try {
                        response.body = request.target.substr(6, request.target.back());
                    }
                    catch (std::out_of_range)
                    {
                        response.body = "";
                        break;
                    }
                    break;
                }
                else if (target == "/user-agent")
                {
                    response.statusLine = response200();
                    Utils::printRequest(request);
                    response.body = request.headers.at(HeaderType::USER_AGENT);
                    break;
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
