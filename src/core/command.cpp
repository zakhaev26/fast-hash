#include "core/command.hpp"
#include "core/parser.hpp"
#include <iostream>
#define __SET__ "SET"
#define __GET__ "GET"
#define __EXPIRE__ "EXPIRE"
#define __TTL__ "TTL"
#define __SETEX__ "SETEX"
#define __DEL__ "DEL"

Command Command::parse(const std::string &line)
{
    Command cmd;
    cmd.args = parser::tokenize(line);
    if (cmd.args.empty())
    {
        cmd.type = INVALID;
        return cmd;
    }

    std::string cmd_name = parser::to_upper(cmd.args[0]);

    if (cmd_name == __SET__)
        cmd.type = SET;
    else if (cmd_name == __GET__)
        cmd.type = GET;
    else if (cmd_name == __DEL__)
        cmd.type = DEL;
    else if (cmd_name == __EXPIRE__)
        cmd.type = EXPIRE;
    else if (cmd_name == __TTL__)
        cmd.type = TTL;
    else if (cmd_name == __SETEX__)
        cmd.type = SETEX;
    else
        cmd.type = INVALID;

    return cmd;
}

void Command::execute(FastHash &store) const
{
    switch (type)
    {
    case SET:
        if (args.size() != 3)
        {
            std::cout << "ERROR: SET usage: SET key value\n";
            break;
        }
        store.set(args[1], args[2]);
        std::cout << "OK\n";
        break;

    case GET:
    {
        if (args.size() != 2)
        {
            std::cout << "ERROR: GET usage: GET key\n";
            break;
        }
        auto val = store.get(args[1]);
        if (val.has_value())
            std::cout << val.value() << "\n";
        else
            std::cout << "(nil)\n";
        break;
    }

    case DEL:
        if (args.size() != 2)
        {
            std::cout << "ERROR: DEL usage: DEL key\n";
            break;
        }
        if (store.del(args[1]))
            std::cout << "OK\n";
        else
            std::cout << "(nil)\n";
        break;

    case EXPIRE:
        if (args.size() != 3)
        {
            std::cout << "ERROR: EXPIRE usage: EXPIRE key seconds\n";
            break;
        }
        try
        {
            int seconds = std::stoi(args[2]);
            if (store.expire(args[1], seconds))
                std::cout << "OK\n";
            else
                std::cout << "(nil)\n";
        }
        catch (...)
        {
            std::cout << "ERROR: invalid seconds\n";
        }
        break;

    case TTL:
        if (args.size() != 2)
        {
            std::cout << "ERROR: TTL usage: TTL key\n";
            break;
        }
        {
            int remaining = store.ttl(args[1]);
            if (remaining == -2)
                std::cout << "(nil)\n";
            else if (remaining == -1)
                std::cout << "-1\n";
            else
                std::cout << remaining << "\n";
        }
        break;

    case SETEX:
        if (args.size() != 4)
        {
            std::cout << "ERROR: SETEX usage: SETEX key seconds value\n";
            break;
        }
        try
        {
            int seconds = std::stoi(args[2]);
            store.set(args[1], args[3], seconds);
            std::cout << "OK\n";
        }
        catch (...)
        {
            std::cout << "ERROR: invalid seconds\n";
        }
        break;

    default:
        std::cout << "ERROR: unknown command\n";
    }
}
