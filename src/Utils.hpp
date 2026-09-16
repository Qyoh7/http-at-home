#pragma once
#include "Utils.hpp"
#include "HttpTypes.hpp"
#include <string>
#include <vector>

enum class LogLevel
{
    INFO,
    WARN,
    ERROR,
    FATAL,
};

namespace Utils
{
    std::string escapeStr(std::string str);
    bool isStringUpper(std::string str);
    bool isStringAlpha(std::string str);

    bool isStringNum(std::string str);
    std::string stringToLower(std::string str);

    void printRequest(Request request);
    void log(std::string msg, LogLevel level = LogLevel::INFO);

    std::string shiftVector(std::vector<std::string> &arr);
    char shiftString(std::string &str);
}
