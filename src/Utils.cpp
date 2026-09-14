#include "Utils.hpp"
#include "HttpTypes.hpp"
#include <magic_enum/magic_enum.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

namespace Utils
{
    bool isStringUpper(std::string str)
    {
        for (char c : str)
        {
            if (!std::isupper(c))
            {
                return false;
            }
        }
        return true;
    }

    bool isStringAlpha(std::string str)
    {
        for (char c : str)
        {
            if (!std::isalpha(c))
            {
                return false;
            }
        }
        return true;
    }

    bool isStringNum(std::string str)
    {
        for (char c : str)
        {
            if (!std::isdigit(c))
            {
                return false;
            }
        }
        return true;
    }

    std::string stringToLower(std::string str)
    {
        std::string ret;
        for (char c : str)
        {
            ret += tolower(c);
        }
        return ret;
    }

    RequestMethod stringToRequestMethod(std::string str)
    {
        if (str == "GET")
        {
            return RequestMethod::GET;
        }
        else if (str == "HEAD")
        {
            return RequestMethod::HEAD;
        }
        else if (str == "POST")
        {
            return RequestMethod::POST;
        }
        else if (str == "PUT")
        {
            return RequestMethod::PUT;
        }
        else if (str == "DELETE")
        {
            return RequestMethod::DELETE;
        }
        else if (str == "CONNECT")
        {
            return RequestMethod::CONNECT;
        }
        else if (str == "OPTIONS")
        {
            return RequestMethod::CONNECT;
        }
        else if (str == "TRACE")
        {
            return RequestMethod::TRACE;
        }
        else if (str == "PATCH")
        {
            return RequestMethod::PATCH;
        }
        else 
        {
            throw std::runtime_error("Unexpected request method " + str);
        }
    }

    HeaderType stringToHeaderType(std::string str)
    {
        if (str == "Host:")
        {
            return HeaderType::HOST;
        }
        else if (str == "User-Agent:")
        {
            return HeaderType::USER_AGENT;
        }
        else if (str == "Accept:")
        {
            return HeaderType::ACCEPT;
        }
        else if (str == "Content-Length:")
        {
            return HeaderType::CONTENT_LENGTH;
        }
        else if (str == "Content-Type:")
        {
            return HeaderType::CONTENT_TYPE;
        }
        else 
        {
            throw std::runtime_error("Unexpected Header Type " + str);
        }
    }

    std::string escapeStr(std::string str)
    {
        std::string escaped;
        for (int i = 0; i < str.length(); i++)
        {
            switch (char c = str.at(i))
            {
                case '\r':
                    escaped += "\\r";
                    continue;
                case '\n':
                    escaped += "\\n";
                    continue;
                default:
                    escaped += c;
                    break;
            }
        }
        return escaped;
    }

    void log(std::string msg, LogLevel level)
    {
        switch (level)
        {
            case LogLevel::INFO:
                std::cout << "[INFO] " << msg << "\n";
                break;

            case LogLevel::WARN:
                std::cout << "[WARN] " << msg << "\n";
                break;
            case LogLevel::ERROR:
                std::cout << "[ERROR] " << msg << "\n";
                break;
            case LogLevel::FATAL:
                std::cout << "[FATAL] " << msg << "\n";
                break;
            default:
                throw std::logic_error("Invalid log level");
        }
    }

    std::string shiftVector(std::vector<std::string> &arr)
    {
        std::string ret = arr.front();
        arr.erase(arr.begin());
        return ret;
    }

    char shiftString(std::string &str)
    {
        char ret = str.front();
        str.erase(str.begin());
        return ret;
    }

    void printRequest(Request request)
    {
        std::cout << "===REQUEST LINE===\n";
        std::cout << "request method: " << magic_enum::enum_name(request.method) << "\n";
        std::cout << "request target: " << request.target << "\n";
        std::cout << "version: " << request.version << "\n";
        std::cout << "===HEADERS===\n";
        std::printf("Number of headers, %zu\n", request.headers.size());
        for (Header h : request.headers)
        {
            if (h.value.index() == VARIENT_INT) 
                std::cout << "type: " << magic_enum::enum_name(h.type) << " value: " << std::get<int>(h.value) << "\n";
            else if (h.value.index() == VARIENT_STRING) 
                std::cout << "type: " << magic_enum::enum_name(h.type) << " value: " << std::get<std::string>(h.value) << "\n";
            else std::cout << "Invalid header value type: header name: " << magic_enum::enum_name(h.type);
        }
        if (request.body.has_value())
        {
            std::cout << "===BODY===\n";
            std::cout << "size: " << request.body.value().length() << "\n";
            std::cout << Utils::escapeStr(request.body.value()) << "\n";
        }
    }
}
