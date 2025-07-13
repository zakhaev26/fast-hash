#include <iostream>
#include <string>
#include "core/command.hpp"
#include "core/fast-hash.hpp"

int main()
{
    FastHash store;
    std::string input;

    std::cout << "Welcome to Fast-Hash CLI. Type EXIT to quit.\n";

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "EXIT")
            break;

        Command cmd = Command::parse(input);
        cmd.execute(store);
    }

    std::cout << "Goodbye!\n";
    return 0;
}
