#include "HttpRequestParser.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include <string>
#include <sys/socket.h>
#include <stdexcept>

namespace HttpRequestParser
{
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
            throw std::runtime_error("Unexpected Header Type \"" + str + "\"");
        }
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

    std::string recvRequest(int client_fd)
    {
        std::string buf;
        buf.resize(1024);
        int responseSizeInBytes = recv(client_fd, buf.data(), buf.size(), 0);
        if (responseSizeInBytes == -1)
        {
            throw std::runtime_error("Failed to receive request from client");
        }
        buf.resize(responseSizeInBytes);
        return buf;
    }

    std::vector<std::string> tokenizeRequest(std::string raw_request)
    {
        std::vector<std::string> words;
        std::string word;
        for (;;)
        {
            switch (char c = raw_request.front())
            {
                case ' ':
                    words.emplace_back(word);
                    word = "";
                    Utils::shiftString(raw_request);
                    break;
                case '\r':
                    if (words.back().compare("\r\n") == 0)
                    {
                        words.emplace_back("\r\n");
                        Utils::shiftString(raw_request);
                        Utils::shiftString(raw_request);
                        goto end;
                    }
                    words.emplace_back(word);
                    word = "";
                    word += Utils::shiftString(raw_request);
                    break;
                case '\n':
                    word += Utils::shiftString(raw_request);
                    words.emplace_back(word);
                    word = "";
                    break;
                default:
                    word += Utils::shiftString(raw_request);
            }
        }
end:
        if (raw_request.length() > 0)
        {
            words.emplace_back(raw_request);
        }
        return words;
    }

    Request parseRequest(std::string rawRequest)
    {
        std::vector<std::string> requestWords = tokenizeRequest(rawRequest);
        Request ret{};
        ret.method = stringToRequestMethod(Utils::shiftVector(requestWords));
        ret.target = Utils::shiftVector(requestWords);
        ret.version = Utils::shiftVector(requestWords);
        Utils::shiftVector(requestWords);

        for (;;)
        {
            if (requestWords.front() == "\r\n")
            {
                Utils::shiftVector(requestWords);
                break;
            }

            HeaderType type = stringToHeaderType(Utils::shiftVector(requestWords));
            std::string value = Utils::shiftVector(requestWords);

            ret.headers[type] = value;

            if (Utils::shiftVector(requestWords) != "\r\n")
            {
                throw std::runtime_error("Malformed Header: missing CRLF");
            }
        }

        if (!requestWords.empty())
        {
            ret.body = requestWords.front();
        }

        return ret;
    }
};
