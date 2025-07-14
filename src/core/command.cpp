// Copyright 2025 Soubhik Gon
#include "core/command.hpp"
#include "core/parser.hpp"
#include <iostream>
#define __SET__ "SET"
#define __GET__ "GET"
#define __EXPIRE__ "EXPIRE"
#define __TTL__ "TTL"
#define __SETEX__ "SETEX"
#define __DEL__ "DEL"
#define __KEYS__ "KEYS"
#define __EXISTS__ "EXISTS"
#define __PERSIST__ "PERSIST"
#define __FLUSHALL__ "FLUSHALL"
#define __SAVE__ "SAVE"

Command Command::parse(const std::string &line) {
  Command cmd;
  cmd.args = parser::tokenize(line);
  if (cmd.args.empty()) {
    cmd.type = INVALID;
    return cmd;
  }

  std::string cmd_name = parser::to_upper(cmd.args[0]);

  if (cmd_name == __SET__)
    cmd.type = Command::Type::SET;
  else if (cmd_name == __GET__)
    cmd.type = Command::Type::GET;
  else if (cmd_name == __DEL__)
    cmd.type = Command::Type::DEL;
  else if (cmd_name == __EXPIRE__)
    cmd.type = Command::Type::EXPIRE;
  else if (cmd_name == __TTL__)
    cmd.type = Command::Type::TTL;
  else if (cmd_name == __SETEX__)
    cmd.type = Command::Type::SETEX;
  else if (cmd_name == __KEYS__)
    cmd.type = Command::Type::KEYS;
  else if (cmd_name == __PERSIST__)
    cmd.type = Command::Type::PERSIST;
  else if (cmd_name == __FLUSHALL__)
    cmd.type = Command::Type::FLUSHALL;
  else if (cmd_name == __SAVE__)
    cmd.type = Command::Type::SAVE;
  else
    cmd.type = INVALID;

  return cmd;
}

void Command::execute(FastHash &store) const {
  switch (type) {
  case Command::Type::SET:
    if (args.size() != 3) {
      std::cout << "ERROR: SET usage: SET key value\n";
      break;
    }
    store.set(args[1], args[2]);
    std::cout << "OK\n";
    break;

  case Command::Type::GET: {
    if (args.size() != 2) {
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

  case Command::Type::DEL:
    if (args.size() != 2) {
      std::cout << "ERROR: DEL usage: DEL key\n";
      break;
    }
    if (store.del(args[1]))
      std::cout << "OK\n";
    else
      std::cout << "(nil)\n";
    break;

  case Command::Type::EXPIRE:
    if (args.size() != 3) {
      std::cout << "ERROR: EXPIRE usage: EXPIRE key seconds\n";
      break;
    }
    try {
      int seconds = std::stoi(args[2]);
      if (store.expire(args[1], seconds))
        std::cout << "OK\n";
      else
        std::cout << "(nil)\n";
    } catch (...) {
      std::cout << "ERROR: invalid seconds\n";
    }
    break;

  case Command::Type::TTL:
    if (args.size() != 2) {
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

  case Command::Type::SETEX:
    if (args.size() != 4) {
      std::cout << "ERROR: SETEX usage: SETEX key seconds value\n";
      break;
    }
    try {
      int seconds = std::stoi(args[2]);
      store.set(args[1], args[3], seconds);
      std::cout << "OK\n";
    } catch (...) {
      std::cout << "ERROR: invalid seconds\n";
    }
    break;

  case Command::Type::KEYS:
    if (args.size() != 2) {
      std::cout << "ERROR: Usage: KEYS pattern\n";
      return;
    }
    for (const auto &key : store.keys(args[1]))
      std::cout << key << "\n";
    break;

  case Command::Type::EXISTS:
    if (args.size() != 2) {
      std::cout << "ERROR: Usage: EXISTS key\n";
      return;
    }
    std::cout << (store.exists(args[1]) ? "1\n" : "0\n");
    break;

  case Command::Type::PERSIST:
    if (args.size() != 2) {
      std::cout << "ERROR: Usage: PERSIST key\n";
      return;
    }
    std::cout << (store.persist(args[1]) ? "1\n" : "0\n");
    break;

  case Command::Type::FLUSHALL:
    if (args.size() != 1) {
      std::cout << "ERROR: FLUSHALL takes no arguments\n";
      return;
    }
    store.flush_all();
    std::cout << "OK\n";
    break;

  case SAVE:
    if (args.size() != 1) {
      std::cout << "ERROR: SAVE takes no arguments\n";
      return;
    }
    if (store.save()) {
      std::cout << "OK\n";
    } else {
      std::cout << "ERROR: Failed to save\n";
    }
    break;

  default:
    std::cout << "ERROR: unknown command\n";
  }
}

// void Command::handle_keys(FastHash &store, const std::vector<std::string>
// &args)
// {
//     if (args.size() != 2)
//     {
//         std::cout << "ERROR: Usage: KEYS pattern\n";
//         return;
//     }

//     std::vector<std::string> matches = store.keys(args[1]);

//     if (matches.empty())
//     {
//         std::cout << "(empty list)\n";
//     }
//     else
//     {
//         for (const auto &k : matches)
//         {
//             std::cout << k << "\n";
//         }
//     }
// }