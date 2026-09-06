#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
enum class HeaderType
{
    HOST,
    USER_AGENT,
    ACCEPT,
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
    RequestMethod method;
    std::string target;
    std::string version;
} RequestLine;

typedef struct
{
    HeaderType type;
    std::string value;
} Header;

typedef struct 
{
    RequestLine requestLine;
    std::vector<Header> headers;
} Request;
