#pragma once
#include <string>
#include <filesystem>

class HttpServer
{
    private:
        const int connection_backlog = 5;
        int port;
        std::filesystem::path root_dir;
        int server_fd;

        int createSocket();
        void bindSocket();
        void setReuse();
        void listenSocket();
        int acceptClient();
        void handleClient(int client_fd);
    public:
        explicit HttpServer();
        ~HttpServer();
        void init(int port, std::string root_dir);
        void run();
};
