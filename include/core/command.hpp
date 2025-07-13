#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "fast-hash.hpp"

class Command
{
public:
    enum Type
    {
        SET,
        GET,
        DEL,
        INVALID
    };
    Type type;
    std::vector<std::string> args;

    static Command parse(const std::string &line);
    void execute(FastHash &store) const;
};
