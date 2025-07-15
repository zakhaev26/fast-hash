// Copyright 2025 Soubhik Gon
#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
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
    ASAVE,
    LOAD,
    INVALID
  };
  Type type;
  std::vector<std::string> args;
  // void handle_keys(FastHash &store, const std::vector<std::string> &tokens);

  static Command parse(const std::string &line);
  void execute(FastHash &store) const;
};
