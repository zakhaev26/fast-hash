#include <iostream>
#include <string>
#include "core/command.hpp"
#include "core/fast-hash.hpp"
int main()
{
    FastHash store;
    std::string line;

    std::cout << "FastHash CLI. Commands: SET, GET, DEL, EXPIRE, TTL, SETEX\n";

    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;
        if (line.empty())
            continue;

        Command cmd = Command::parse(line);
        if (cmd.type == Command::INVALID)
        {
            std::cout << "ERROR: Invalid command\n";
            continue;
        }

        cmd.execute(store);
    }

    store.stop();
    return 0;
}
