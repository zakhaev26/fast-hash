#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <functional>
#include <optional>

class TTLManager
{
public:
    TTLManager();
    ~TTLManager();
    void add_expiration(
        const std::string &key,
        std::chrono::steady_clock::time_point expire_time);
    void remove_expiration(
        const std::string &key);
    bool expired(
        const std::string &key);
    void stop();
    void set_expire_callback(std::function<void(const std::string &)> cb);
    // std::function<void(const std::string &)> on_expire_callback_;
    // void set_expire_callback(std::function<void(const std::string &)> cb);
    std::optional<std::chrono::steady_clock::time_point> get_expiry_time(const std::string &key);
    bool has_expiration(const std::string& key);

private:
    struct expire_entry
    {
        std::string key;

        std::chrono::steady_clock::time_point expire_time;

        bool operator>(const expire_entry &other) const
        {
            return this->expire_time > other.expire_time;
        }
    };

    std::priority_queue<expire_entry, std::vector<expire_entry>, std::greater<>>
        expiry_heap_;

    std::unordered_map<std::string, std::chrono::steady_clock::time_point> expiry_map_;

    std::mutex mutex_;

    std::condition_variable cv_;

    bool stop_flag_;

    std::thread sweeper_thread_;

    void sweeper();

    std::function<void(const std::string &key)> on_expire_callback_;
};