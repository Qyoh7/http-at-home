#include "HttpServer.hpp"

int main()
{
    HttpServer server(8080);
    server.init();
    server.run();

    return 0;
}
