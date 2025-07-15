// Copyright 2025 Soubhik Gon
#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace parser
{
    std::vector<std::string> tokenize(const std::string &line);
    std::string to_upper(const std::string &str);
} // namespace parser
