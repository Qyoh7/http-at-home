#include "HttpServer.hpp"
#include "HttpRequestParser.hpp"
#include "Utils.hpp"
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <thread>
#include <cstring>
#include <stdexcept>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

HttpServer::HttpServer(HttpRouter& router) : router(router)
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
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
    if (client_fd < 0)
    {
        throw std::runtime_error(std::string("Failed to accept client: ") + std::strerror(errno));
    }
    Utils::log("Client connected", LogLevel::INFO);
    return client_fd;
}
std::string HttpServer::headerTypeToString(const HeaderType &type)
{
    switch (type)
    {
        case HeaderType::HOST:
            return "Host:";
        case HeaderType::CONTENT_TYPE:
            return "Content-Type:";
        case HeaderType::CONTENT_LENGTH:
            return "Content-Length:";
        case HeaderType::ACCEPT:
            return "Accept:";
        case HeaderType::USER_AGENT:
            return "User-Agent:";
        default:
            throw std::logic_error(std::string("Invalid header type when converting to string: ") + std::string(magic_enum::enum_name(type)));
    }
}

std::string HttpServer::responseToRawString(const Response &response)
{
    std::stringstream raw_response;
    raw_response << response.statusLine << "\r\n";
    for (auto it = response.headers.begin(); it != response.headers.end(); it++)
    {
        raw_response << headerTypeToString(it->first);
        raw_response << " " << it->second << "\r\n";
    }
    raw_response << "\r\n";
    if (response.body.has_value())
    {
        raw_response << response.body.value();
    }
    return raw_response.str();
}

void HttpServer::sendResponse(Response response, int client_fd)
{
    std::string raw_response = responseToRawString(response);
    if (send(client_fd, raw_response.data(), raw_response.size(), 0) < 0)
    {
        throw std::runtime_error(std::string("Failed to send response: ") + std::strerror(errno));
    }
    Utils::log("Sent back: " + Utils::escapeStr(raw_response), LogLevel::INFO);
}

void HttpServer::handleClient(int client_fd)
{
    std::string rawRequest = HttpRequestParser::recvRequest(client_fd);
    Request request = HttpRequestParser::parseRequest(rawRequest);
    Response response = router.routeRequest(request);
    sendResponse(response, client_fd);
}

void HttpServer::init(int port)
{
    this->port = port;
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
        std::thread([this, client_fd]()
                    {
                handleClient(client_fd);
                close(client_fd); })
            .detach();
    }
}
