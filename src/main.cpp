#include "HttpServer.hpp"
#include "Utils.hpp"
#include <complex>
#include <cstdlib>
#include <string>
#include <string.h>
#include <unordered_map>

enum class ArgType
{
    DIRECTORY,
    PORT
};

int main(int argc, char* argv[])
{
    size_t arg_index = 1;
    std::unordered_map<ArgType, std::string> args;
    while (arg_index < argc)
    {
        if (strcmp(argv[arg_index], "--directory") == 0)
        {
            arg_index++;
            args[ArgType::DIRECTORY] = argv[arg_index++];
        }
        else if (strcmp(argv[arg_index], "-d") == 0)
        {
            arg_index++;
            args[ArgType::DIRECTORY] = argv[arg_index++];
        }
        else if (strcmp(argv[arg_index], "--port") == 0)
        {
            arg_index++;
            args[ArgType::PORT] = argv[arg_index++];
        }
        else if (strcmp(argv[arg_index], "-p") == 0)
        {
            arg_index++;
            args[ArgType::PORT] = argv[arg_index++];
        }
    }
    HttpServer server;
    if (args.contains(ArgType::PORT))
    {
        if (!Utils::isStringNum(args[ArgType::PORT]))
        {
            Utils::log("Port must be a number, dumbass", LogLevel::FATAL);
            return EXIT_FAILURE;
        }
        if (args.contains(ArgType::DIRECTORY))
        {
            server.init(stoi(args[ArgType::PORT]), args[ArgType::DIRECTORY]);
        }
        else {
            Utils::log("Incorrect usage. Correct usage: http-server --port <port> --directory <dir>", LogLevel::FATAL);
            return EXIT_FAILURE;
        }
        
    }
    else {
        Utils::log("Incorrect usage. Correct usage: http-server --port <port> --directory <dir>", LogLevel::FATAL);
        return EXIT_FAILURE;
    }
    
    server.run();

    return 0;
}
