#include "HttpServer.hpp"
#include "HttpRequestParser.hpp"
#include "HttpRequestHandler.hpp"
#include "Utils.hpp"
#include <cstring>
#include <stdexcept>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

HttpServer::HttpServer(int port) : port(port)
{

}

HttpServer::~HttpServer()
{
    close(server_fd);
}

int HttpServer::createSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    return sock;
}

void HttpServer::setReuse()
{
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }
}

void HttpServer::bindSocket()
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        throw std::runtime_error("Failed to bind to port " + std::to_string(port));
    }
}

void HttpServer::listenSocket()
{
    if (listen(server_fd, connection_backlog) != 0) {
        throw std::runtime_error(std::string("listen failed"));
    }
}

int HttpServer::acceptClient()
{
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    Utils::log("Waiting for a client to connect", LogLevel::INFO);
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if (client_fd < 0)
    {
        throw std::runtime_error(std::string("Failed to accept client: ") + std::strerror(errno));
    }
    Utils::log("Client connected", LogLevel::INFO);
    return client_fd;
}

void HttpServer::handleClient(int client_fd)
{
    std::string rawRequest = HttpRequestParser::recvRequest(client_fd);
    Request request = HttpRequestParser::parseRequest(rawRequest);
    HttpRequestHandler::handleRequest(request, client_fd);
}

void HttpServer::init()
{
    server_fd = createSocket();

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    setReuse();
    bindSocket();
    listenSocket();
    Utils::log("Listening on port " + std::to_string(port), LogLevel::INFO);
}


void HttpServer::run()
{
    for (;;)
    {
        int client_fd = acceptClient();
        handleClient(client_fd);
        close(client_fd);
    }
}
