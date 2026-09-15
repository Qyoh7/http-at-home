#include "HttpRequestParser.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include <string>
#include <sys/socket.h>
#include <stdexcept>

namespace HttpRequestParser
{
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
        ret.method = Utils::stringToRequestMethod(Utils::shiftVector(requestWords));
        ret.target = Utils::shiftVector(requestWords);
        ret.version = Utils::shiftVector(requestWords);
        Utils::shiftVector(requestWords);

        for (;;)
        {
            Header header{};
            header.type = Utils::stringToHeaderType(Utils::shiftVector(requestWords));
            header.value = Utils::shiftVector(requestWords);

            ret.headers.emplace_back(header);

            if (Utils::shiftVector(requestWords) != "\r\n")
            {
                throw std::runtime_error("Malformed Header: missing CRLF");
            }
            if (requestWords.front() == "\r\n")
            {
                Utils::shiftVector(requestWords);
                break;
            }
        }

        if (!requestWords.empty())
        {
            ret.body = requestWords.front();
        }

        return ret;
    }
};
