// Copyright 2025 Soubhik Gon
#pragma once
#include "ttl-manager.hpp"
#include "persistence/aof/aof.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <thread>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
class FastHash
{
public:
  // FastHash();
  FastHash(const std::string &aof_path = "appendonly.aof", AOFSyncPolicy policy = AOFSyncPolicy::ALWAYS);
  ~FastHash();
  bool set(const std::string &key, const std::string &value,
           std::optional<int> ttl_seconds = std::nullopt);
  // bool get(const std::string &key, std::string &value) const;
  std::optional<std::string> get(const std::string &key);
  bool del(const std::string &key);
  bool expire(const std::string &key, int seconds);
  int ttl(const std::string &key);
  std::vector<std::string> keys(const std::string &pattern = "*");
  bool exists(const std::string &key);
  bool persist(const std::string &key);
  void flush_all();
  bool save(const std::string &filename = "dump.json") const;
  bool save_async(const std::string &filename = "dump.json") const;
  nlohmann::json serialize() const;
  bool load(const std::string &filepath = "dump.json");
  // void enable_aof(const std::string &path, AOFSyncPolicy policy = AOFSyncPolicy::EVERYSEC);
  // bool load_from_aof(const std::string &path);
  void replayAOF(const std::string &filepath);
  void stop();

private:
  struct Value
  {
    std::string data;
  };

  std::unordered_map<std::string, Value> store_;
  TTLManager ttl_manager_;
  mutable std::mutex mutex_;
  // std::unique_ptr<AOFLogger> aof_log_;
  AOFLogger aof_logger_;
  bool is_aof_loading_;
  // void remove_if_expired(const std::string &key);
};