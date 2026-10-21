#include "../../src/HttpRouter.hpp"
#include "../../src/HttpServer.hpp"
#include <fstream>

Response getRoot(Request request)
{
    Response response;
    std::ifstream fin;
    fin.open("index.html");
    if (fin.is_open())
    {
        
    }
    response.headers[HeaderType::CONTENT_LENGTH];
    return response;
}

int main()
{
    HttpRouter router;
    HttpServer server(router);
    router.addRoute(RequestMethod::GET, "/", getRoot);
    server.init(8080);
    server.run();

}

