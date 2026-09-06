#pragma once
#include "RequestTypes.h"
#include "../include/magic_enum/magic_enum.hpp"
#include <iostream>
#include <string>

namespace Utils
{
    static std::string escapeStr(std::string str)
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

    static bool isStringUpper(std::string str)
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
    static bool isStringAlpha(std::string str)
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

    static RequestMethod stringToRequestMethod(std::string str)
    {
        if (str.compare("GET") == 0)
        {
            return RequestMethod::GET;
        }
        else if (str.compare("HEAD") == 0)
        {
            return RequestMethod::HEAD;
        }
        else if (str.compare("POST") == 0)
        {
            return RequestMethod::POST;
        }
        else if (str.compare("PUT") == 0)
        {
            return RequestMethod::PUT;
        }
        else if (str.compare("DELETE") == 0)
        {
            return RequestMethod::DELETE;
        }
        else if (str.compare("CONNECT") == 0)
        {
            return RequestMethod::CONNECT;
        }
        else if (str.compare("OPTIONS") == 0)
        {
            return RequestMethod::CONNECT;
        }
        else if (str.compare("TRACE") == 0)
        {
            return RequestMethod::TRACE;
        }
        else if (str.compare("PATCH") == 0)
        {
            return RequestMethod::PATCH;
        }
        else 
        {
            std::cerr << "[ERROR] Unexpected request method " << str << "\n";
            exit(EXIT_FAILURE);
        }
    }

    static HeaderType stringToHeaderType(std::string str)
    {
        if (str.compare("Host:") == 0)
        {
            return HeaderType::HOST;
        }
        else if (str.compare("User-Agent:") == 0)
        {
            return HeaderType::USER_AGENT;
        }
        else if (str.compare("Accept:") == 0)
        {
            return HeaderType::ACCEPT;
        }
        else 
        {
            std::cerr << "[ERROR] Unexpected Header Type " << str << "\n";
            exit(EXIT_FAILURE);
        }
    }

    static void printRequest(Request request)
    {
        std::cout << "request method: " << magic_enum::enum_name(request.requestLine.method) << "\n";
        std::cout << "request target: " << request.requestLine.target << "\n";
        std::cout << "version: " << request.requestLine.version << "\n";
        std::cout << "===HEADERS===\n";
        std::printf("Number of headers, %zu\n", request.headers.size());
        for (Header h : request.headers)
        {
            std::cout << "type: " << magic_enum::enum_name(h.type) << " value: " << h.value << "\n";
        }
    }
}
