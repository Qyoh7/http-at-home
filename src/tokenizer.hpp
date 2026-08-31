#include <cctype>
#include <sstream>
#include <strstream>
#include <string>
#include <vector>
#include <optional>

enum class TokenType
{
    REQUEST,
    TARGET,
    PROTO,
    HOST,
    USER_AGENT,
    ACCEPT
};

typedef struct 
{
    TokenType type;
    std::optional<std::string> value;
} Token;

class Tokenizer
{
    private:
        const std::string m_request;
        size_t m_index;

        char peek()
        {
            return m_request[m_index];
        }
        char consume()
        {
            return m_request[m_index++];
        }
        static std::string escapeString(const std::string& str)
        {
            std::stringstream ss;
            for (char c : str)
            {
                switch (c)
                {
                    case '\r': 
                        ss << "\\r";
                    case 
                }
            }
        }
    public:
        explicit Tokenizer(const std::string& request) : m_request(std::move(request)) {}

        std::vector<std::string> splitString(std::string str, const std::string delim)
        {
            size_t delimCount = delim.size();
        }

        std::vector<Token> tokenizeRequest()
        {
            std::string buf;
            while (m_request.substr(m_index, m_index + 3) != "\r\n")
            {
                if (std::isspace(peek()))
                {

                }
                buf += consume();
            }
        }

        std::vector<Token> tokenizeHeader()
        {

        }

        std::vector<Token> tokenize()
        {
            std::vector<Token> tokens;

        }

};
