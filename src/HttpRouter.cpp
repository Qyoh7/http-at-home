#include "HttpRouter.hpp"
#include "HttpResponses.hpp"
#include "HttpTypes.hpp"
#include "Utils.hpp"
#include "magic_enum/magic_enum.hpp"
#include <functional>
/*
this class adds routes to an internal database, 
then when a request is recieved from the client, 
it sends back the appropriate response returned by a callback.
*/

HttpRouter::HttpRouter()
{

}

void HttpRouter::addRoute(RequestMethod method, std::string path, std::function<Response(Request)> callback)
{
    Utils::log("Registering Route " + path + " for method " + std::string(magic_enum::enum_name(method)));
    routes[(Route){path, method}] = callback;
}

Response HttpRouter::routeRequest(const Request& request)
{
    Utils::printRequest(request);
    Utils::log("size of routes: " + std::to_string(routes.size()));
    auto it = routes.find((Route){request.target, request.method});
    if (it == routes.end()) return (Response){.statusLine = response404()};

    Utils::log(it->second ? "callback exists" : "callback is EMPTY");
    return (it)->second(request);
}
