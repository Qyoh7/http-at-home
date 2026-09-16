#include "HttpRequestHandler.hpp"
#include "HttpResponses.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include <filesystem>
#include <cstring>
#include <magic_enum/magic_enum.hpp>
#include <fstream>
#include <stdexcept>
#include <string.h>
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
                return "Content-Type:";
            case HeaderType::CONTENT_LENGTH:
                return "Content-Length:";
            case HeaderType::ACCEPT:
                return "Accept:";
            case HeaderType::USER_AGENT:
                return "User-Agent:";
            default:
                throw std::logic_error(std::string("Invalid header type when converting to string: ") + std::string(magic_enum::enum_name(type)));
        }
    }

    std::string responseToRawString(const Response& response)
    {
        std::stringstream raw_response;
        raw_response << response.statusLine << "\r\n";
        for (auto it = response.headers.begin(); it != response.headers.end(); it++)
        {
            raw_response << headerTypeToString(it->first);
            raw_response << " " << it->second<< "\r\n";
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

    std::string readFileToString(std::filesystem::path path)
    {
        std::ifstream f;
        std::string line;
        std::string file;
        f.open(path);
        if (!f.is_open())
        {
            throw std::runtime_error(std::string("Failed to open file for reading: ") + strerror(errno));
        }

        while (std::getline(f, line))
        {
            file += line;
            file += "\n";
        }
        f.close();
        return file;
    }

    void writeStringToFile(std::string str, std::filesystem::path file)
    {
        std::ofstream fout;
        fout.open(file);
        if (!fout.is_open())
        {
            throw std::runtime_error(std::string("Failed to open file for writing: ") + strerror(errno));
        }
        fout << str;
        fout.close();
    }

    void handleRequest(const Request& request, std::filesystem::path root_dir, int client_fd)
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
                        response.body = target.substr(6, target.back());
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
                else if (target.substr(0, 6) == "/files")
                {
                    std::filesystem::path filePath = root_dir / target.substr(7);
                    std::string file;
                    try {
                        Utils::log("Trying to open file " + static_cast<std::string>(filePath));
                        file = readFileToString(filePath);
                    }
                    catch (std::runtime_error e) {
                        Utils::log(e.what(), LogLevel::ERROR);
                        response.statusLine = response404();
                        break;
                    }
                    response.statusLine = response200();
                    Utils::log("setting content length");
                    response.headers[HeaderType::CONTENT_LENGTH] = std::to_string(file.size());
                    Utils::log("set content length");
                    response.body = file;
                    break;
                }
                break;
            case RequestMethod::POST:
                {
                    std::filesystem::path pth;
                    pth = root_dir / target.substr(7);
                    writeStringToFile(request.body.value(), pth);
                    response.statusLine = response201();
                    break;
                }
            default:
                throw std::runtime_error(std::string("Invalid HTTP Method: ") + magic_enum::enum_name(request.method).data());
                break;
        }
        sendResponse(response, client_fd);
    }
}
