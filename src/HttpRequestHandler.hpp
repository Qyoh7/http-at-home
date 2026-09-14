#pragma once
#include "HttpTypes.hpp"

namespace HttpRequestHandler
{
    void handleRequest(const Request& request, int client_fd);
}
