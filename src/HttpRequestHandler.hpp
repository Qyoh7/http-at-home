#pragma once
#include "HttpTypes.hpp"
#include <filesystem>

namespace HttpRequestHandler
{
    void handleRequest(const Request& request, std::filesystem::path root_dir, int client_fd);
}
