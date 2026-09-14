#pragma once
#include "Utils.hpp"
#include "HttpTypes.hpp"
#include <string>
#include <vector>

namespace Utils
{
    std::string escapeStr(std::string str);
    bool isStringUpper(std::string str);
    bool isStringAlpha(std::string str);

    bool isStringNum(std::string str);
    std::string stringToLower(std::string str);

    RequestMethod stringToRequestMethod(std::string str);
    HeaderType stringToHeaderType(std::string str);
    void printRequest(Request request);

    std::string shiftVector(std::vector<std::string> &arr);
    char shiftString(std::string &str);
}
