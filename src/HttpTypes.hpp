#pragma once
#include <optional>
#include <string>
#include <variant>
#include <vector>

enum {
    VARIENT_STRING,
    VARIENT_INT,
};

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

typedef struct
{
    HeaderType type;
    std::string value;
} Header;

typedef struct 
{
    RequestMethod method;
    std::string target;
    std::string version;
    std::vector<Header> headers;
    std::optional<std::string> body;
} Request;

typedef struct 
{
    std::string statusLine;
    std::optional<std::vector<Header>> headers;
    std::optional<std::string> body;
} Response;
