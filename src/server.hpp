#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

class Server
{
    private:
        std::string m_request;
        int m_client_fd;
    public:
        Server(const int& client_fd)
        {
            m_request.resize(1024);
        }

        void getRequest()
        {
            recv(m_client_fd, m_request.data(), m_request.length(), 0);
        }

        std::vector<std::string> splitString(const std::string &str, const std::string &delim)
        {
            std::vector<std::string> lines;
            std::string tmp;

            for (int i = 0; i < str.length(); i++)
            {
                if (str.at(i) == '\\')
                {
                    if (str.substr(i, i + delim.length()) == delim)
                    {
                        lines.push_back(tmp);
                        tmp = "";
                        i += delim.length() - 1;
                        continue;
                        if (str.substr(i + delim.length(), i + (delim.length() * 2)) == delim)
                        {
                            break;
                        }
                    }
                }
                tmp += str.at(i);
            }
            return lines;
        }

        // void oldparser()
        // {
        //     std::string buf;
        //     buf.resize(1024);
        //     std::string part;
        //     std::string request;
        //     std::string target;
        //     bool gotRequest = false;
        //     bool gotTarget = false;
        //
        //     const char RESPONSE_200[] = "HTTP/1.1 200 OK\r\n\r\n";
        //     const char RESPONSE_404[] = "HTTP/1.1 404 Not Found\r\n\r\n";
        //
        //     recv(client_fd, buf.data(), 1024, 0);
        //
        //     std::cout << "\ngot data:\n" << buf;
        //
        //     for (int i = 0; i < buf.size(); i++) 
        //     {
        //         if (std::isspace(buf.at(i)))
        //         {
        //             if (!gotRequest) 
        //             {
        //                 request = part;
        //                 gotRequest = true;
        //                 std::cout << "Got request " << request << "\n";
        //             }
        //             else if (gotRequest && !gotTarget) 
        //             {
        //                 target = part;
        //                 gotTarget = true;
        //                 std::cout << "Got target " << target << "\n";
        //             }
        //             part = "";
        //             continue;
        //         }
        //         part += buf.at(i);
        //     }
        //
        //     if (request == "GET")
        //     {
        //         if (target == "/")
        //         {
        //             send(client_fd, RESPONSE_200, sizeof(RESPONSE_200), 0);
        //         }
        //         else if (target.substr(0, 5) == "/echo")
        //         {
        //             std::string echoString = target.substr(6, target.size()).data();
        //             std::stringstream response;
        //             response << RESPONSE_200 << "Content-Type: text/plain\r\nContent-Length: " << echoString.length() << "\r\n\r\n" << echoString;
        //
        //             std::cout << "response to echo request: " << response.str() << "\n";
        //
        //             send(client_fd, response.str().c_str(), response.str().length(), 0);
        //         }
        //         else
        //         {
        //             send(client_fd, RESPONSE_404, sizeof(RESPONSE_404), 0);
        //         }
        //
        //     }
        //
        // }

};
