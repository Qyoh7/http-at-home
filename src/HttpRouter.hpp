#pragma once
#include "HttpTypes.hpp"
#include <unordered_map>
#include <functional>

class HttpRouter
{
    private:
        std::unordered_map<Route, std::function<Response(Request)>> routes;
    public:
        explicit HttpRouter();
        void addRoute(RequestMethod method, std::string route, std::function<Response(Request)> callback);
        Response routeRequest(const Request& request);
};
