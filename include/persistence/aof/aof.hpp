#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <cstdio>

enum class AOFSyncPolicy
{
    ALWAYS,
    EVERYSEC,
    NONE
};

class AOFLogger
{
public:
    AOFLogger(const std::string &path, AOFSyncPolicy policy = AOFSyncPolicy::ALWAYS);
    ~AOFLogger();

    void log(const std::string &entry);

private:
    std::ofstream file_;
    FILE *c_file_;
    std::mutex mu_;
    AOFSyncPolicy sync_policy_;
};
