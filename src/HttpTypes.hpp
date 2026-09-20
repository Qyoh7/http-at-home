#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

enum class HeaderType
{
    HOST,
    USER_AGENT,
    ACCEPT,
    CONTENT_LENGTH,
    CONTENT_TYPE,
};

enum class RequestMethod
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

struct Route
{
    std::string path;
    RequestMethod method;
    bool operator==(const Route &other) const
    {
        if ((path == other.path) && (method == other.method)) return true;
        return false;
    }
};

namespace std
{
    template <>
        struct hash<Route> {
            std::size_t operator()(const Route& r) const {
                // Combine the hashes of individual members
                std::size_t h1 = std::hash<std::string>{}(r.path);
                std::size_t h2 = std::hash<RequestMethod>{}(r.method);

                // A simple bitwise combination trick to mix hashes
                return h1 ^ (h2 << 1); 
            }
        };
}

struct Request
{
    RequestMethod method;
    std::string target;
    std::string version;
    std::unordered_map<HeaderType, std::string> headers;
    std::optional<std::string> body;
};

struct Response
{
    std::string statusLine;
    std::unordered_map<HeaderType, std::string> headers;
    std::optional<std::string> body;
};
