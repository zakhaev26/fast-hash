#pragma once
#include <unordered_map>
#include <string>
#include <optional>
#include "ttl-manager.hpp"
class FastHash
{
    // private:
    //     std::unordered_map<std::string, std::string> store;

public:
    FastHash();
    ~FastHash();
    bool set(const std::string &key, const std::string &value, std::optional<int> ttl_seconds = std::nullopt);
    // bool get(const std::string &key, std::string &value) const;
    std::optional<std::string> get(const std::string &key);
    bool del(const std::string &key);
    bool expire(const std::string &key, int seconds);
    int ttl(const std::string &key);
    std::vector<std::string> keys(const std::string &pattern = "*");
    bool exists(const std::string &key);
    bool persist(const std::string &key);
    void flush_all();
    void stop();

private:
    struct Value
    {
        std::string data;
    };

    std::unordered_map<std::string, Value> store_;
    TTLManager ttl_manager_;
    std::mutex mutex_;

    void remove_if_expired(const std::string &key);
};