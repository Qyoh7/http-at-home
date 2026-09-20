#include "HttpServer.hpp"
#include "HttpResponses.hpp"
#include "HttpRouter.hpp"

Response getRoot(Request request)
{
    Response response{};
    response.statusLine = response200();
    response.body = "This tests POST " + request.target;
    return response;
}

int main(int argc, char* argv[])
{
    HttpRouter router;
    HttpServer server(router);
    router.addRoute(RequestMethod::GET, "/abc/def", getRoot);
    server.init(8080);
    server.run();

    return 0;
}
