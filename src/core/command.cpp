#include "core/command.hpp"
#include <iostream>

Command Command::parse(const std::string &line)
{
    std::istringstream iss(line);
    std::string word;
    std::vector<std::string> tokens;

    while (iss >> word)
        tokens.push_back(word);

    Command cmd;
    if (tokens.empty())
    {
        cmd.type = INVALID;
        return cmd;
    }

    std::string cmdType = tokens[0];
    for (auto &c : cmdType)
    {
        c = toupper(c);
    }

    if (cmdType == "SET")
        cmd.type = SET;
    else if (cmdType == "GET")
        cmd.type = GET;
    else if (cmdType == "DEL")
        cmd.type = DEL;
    else
        cmd.type = INVALID;

    tokens.erase(tokens.begin()); // remove the command
    cmd.args = tokens;
    return cmd;
}

void Command::execute(FastHash &store) const
{
    switch (type)
    {
    case SET:
        if (args.size() != 2)
        {
            std::cout << "Usage: SET key value\n";
        }
        else
        {
            store.set(args[0], args[1]);
            std::cout << "OK\n";
        }
        break;
    case GET:
        if (args.size() != 1)
        {
            std::cout << "Usage: GET key\n";
        }
        else
        {
            std::string value;
            if (store.get(args[0], value))
            {
                std::cout << value << "\n";
            }
            else
            {
                std::cout << "(nil)\n";
            }
        }
        break;
    case DEL:
        if (args.size() != 1)
        {
            std::cout << "Usage: DEL key\n";
        }
        else
        {
            if (store.del(args[0]))
            {
                std::cout << "1\n";
            }
            else
            {
                std::cout << "0\n";
            }
        }
        break;
    default:
        std::cout << "Unknown command\n";
    }
}
