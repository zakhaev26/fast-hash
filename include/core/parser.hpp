// Copyright 2025 Soubhik Gon
#pragma once
#include <string>
#include <vector>

namespace parser {
std::vector<std::string> tokenize(const std::string &line);
std::string to_upper(const std::string &str);
} // namespace parser
