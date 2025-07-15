// Copyright 2025 Soubhik Gon
#include "core/parser.hpp"

namespace parser {

std::vector<std::string> tokenize(const std::string &line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

std::string to_upper(const std::string &str) {
  std::string upper_str = str;
  std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return upper_str;
}

} // namespace parser

// CommandType parse_command(const std::string &input, std::vector<std::string>
// &tokens)
// {
//     std::istringstream iss(input);
//     std::string token;

//     while (iss >> token)
//     {
//         tokens.push_back(token);
//     }

//     if (tokens.empty())
//         return CommandType::INVALID;

//     std::string cmd = tokens[0];
//     std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

//     if (cmd == "SET")
//         return CommandType::SET;
//     if (cmd == "GET")
//         return CommandType::GET;
//     if (cmd == "DEL")
//         return CommandType::DEL;
//     if (cmd == "EXPIRE")
//         return CommandType::EXPIRE;
//     if (cmd == "TTL")
//         return CommandType::TTL;
//     if (cmd == "SETEX")
//         return CommandType::SETEX;
//     if (cmd == "KEYS")
//         return CommandType::KEYS;

//     return CommandType::INVALID;
// }
