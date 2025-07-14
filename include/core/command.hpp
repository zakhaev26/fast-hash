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
        SETEX,
        GET,
        DEL,
        EXPIRE,
        TTL,
        KEYS,
        EXISTS,
        PERSIST,
        FLUSHALL,
        SAVE,
        INVALID
    };
    Type type;
    std::vector<std::string> args;
    // void handle_keys(FastHash &store, const std::vector<std::string> &tokens);

    static Command parse(const std::string &line);
    void execute(FastHash &store) const;
};
