#pragma once
#include "HttpRouter.hpp"
#include <string>

class HttpServer
{
    private:
        HttpRouter& router;
        const int connection_backlog = 5;
        int port;
        int server_fd;

        std::string headerTypeToString(const HeaderType &type);
        std::string responseToRawString(const Response &response);
        void sendResponse(Response response, int client_fd);

        int createSocket();
        void bindSocket();
        void setReuse();
        void listenSocket();
        int acceptClient();
        void handleClient(int client_fd);
    public:
        explicit HttpServer(HttpRouter& router);
        ~HttpServer();
        void init(int port);
        void run();
};
