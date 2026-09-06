#include "RequestTypes.h"
#include "Utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <assert.h>

class Parser
{
    private:
        static std::string shift(std::vector<std::string> &arr)
        {
            std::string ret = arr.front();
            arr.erase(arr.begin());
            return ret;
        }

        static std::string peek(std::vector<std::string> &arr, int offset = 1)
        {
            return arr.at(offset);
        }

    public:
        static std::vector<std::string> tokenize(std::string str)
        {
            std::vector<std::string> words;
            std::string word;
            for (char c : str)
            {
                switch (c)
                {
                    case ' ':
                        words.push_back(word);
                        word = "";
                        break;
                    case '\r':
                        if (words.back().compare("\r\n") == 0)
                        {
                            words.push_back("\r\n");
                            goto end;
                        }
                        words.push_back(word);
                        word = "";
                        word += c;
                        break;
                    case '\n':
                        word += c;
                        words.push_back(word);
                        word = "";
                        break;
                    default:
                        word += c;
                }
            }
end:
            return words;
        }

        static Request parseRequest(std::string req)
        {
            std::vector<std::string> requestWords = tokenize(req);
            Request ret{};
            ret.requestLine.method = Utils::stringToRequestMethod(shift(requestWords));
            ret.requestLine.target = shift(requestWords);
            ret.requestLine.version = shift(requestWords);
            shift(requestWords);

            while (!(requestWords.front().compare("\r\n") == 0))
            {
                Header header{};
                header.type = Utils::stringToHeaderType(shift(requestWords));
                header.value = shift(requestWords);
                ret.headers.push_back(header);
                std::cout << "\n\n";
                assert(shift(requestWords).compare("\r\n") == 0 && "Malformed Header: missing CRLF");
            }
            return ret;
        }

        static Request recvRequest(int client_fd)
        {
            std::string buf;
            buf.resize(1024);
            int responseSizeInBytes = recv(client_fd, buf.data(), buf.size(), 0);
            assert(responseSizeInBytes != -1);
            buf.resize(responseSizeInBytes);
            return parseRequest(buf);
        }
};
