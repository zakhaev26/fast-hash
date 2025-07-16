#include "persistence/aof/aof.hpp"
#include <iostream>
#include <unistd.h>

AOFLogger::AOFLogger(const std::string &path, AOFSyncPolicy policy)
    : sync_policy_(policy), c_file_(nullptr) {
  file_.open(path, std::ios::app);
  if (!file_.is_open()) {
    throw std::runtime_error("Failed to open AOF log file: " + path);
  }

  c_file_ = std::fopen(path.c_str(), "a");
  if (!c_file_) {
    throw std::runtime_error("Failed to open C-style AOF file handle.");
  }
}

AOFLogger::~AOFLogger() {
  if (file_.is_open()) {
    file_.flush();
    file_.close();
  }

  if (c_file_) {
    std::fflush(c_file_);
    std::fclose(c_file_);
  }
}

void AOFLogger::log(const std::string &entry) {
  std::lock_guard<std::mutex> lock(mu_);
  // std::cout << "logging in aof..." << std::endl;
  file_ << entry << "\n";

  switch (sync_policy_) {
  case AOFSyncPolicy::ALWAYS:
    file_.flush();
    if (c_file_) {
      fflush(c_file_);
      fsync(fileno(c_file_));
    }
    break;

  case AOFSyncPolicy::EVERYSEC:
    // optionally background thread
    break;

  case AOFSyncPolicy::NONE:
    // no fsync at all
    break;
  }
}
