#pragma once
#include <arpa/inet.h>

class HttpServer
{
    private:
        const int connection_backlog = 5;
        const int port;
        int server_fd;

        int createSocket();
        void bindSocket();
        void setReuse();
        void listenSocket();
        int acceptClient();
        void handleClient(int client_fd);
    public:
        explicit HttpServer(int port);
        ~HttpServer();
        void init();
        void run();
};
