#include "core/parser.hpp"
#include <sstream>
#include <algorithm>

namespace parser
{

    std::vector<std::string> tokenize(const std::string &line)
    {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token)
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string to_upper(const std::string &str)
    {
        std::string upper_str = str;
        std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(),
                       [](unsigned char c)
                       { return std::toupper(c); });
        return upper_str;
    }

}
