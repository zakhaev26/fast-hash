#include "core/ttl-manager.hpp"

TTLManager::TTLManager() : stop_flag_(false)
{
    this->sweeper_thread_ = std::thread(&TTLManager::sweeper, this);
}

TTLManager::~TTLManager()
{
    this->stop();
}

void TTLManager::stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_flag_ = true;
    }
    cv_.notify_all();
    if (sweeper_thread_.joinable())
    {
        sweeper_thread_.join();
    }
}

void TTLManager::add_expiration(
    const std::string &key,
    std::chrono::steady_clock::time_point expiry_time)
{
    std::lock_guard<std::mutex> lock(this->mutex_);
    expiry_map_[key] = expiry_time;
    expiry_heap_.push(expire_entry{key, expiry_time});
    cv_.notify_all();
}

void TTLManager::remove_expiration(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);
    expiry_map_.erase(key);
    // Cannot efficiently remove from priority_queue, so stale entries handled in sweeper
}

bool TTLManager::expired(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);

    auto now = std::chrono::steady_clock::now();

    auto it = expiry_map_.find(key);
    if (it == expiry_map_.end())
    {
        return false;
    }

    if (now >= it->second)
    {
        expiry_map_.erase(it);
        return true;
    }

    return false;
}

void TTLManager::sweeper()
{
    std::unique_lock<std::mutex> lock(this->mutex_);

    while (!stop_flag_)
    {
        if (expiry_heap_.empty())
        {
            cv_.wait(lock);
        }
        else
        {
            auto next = expiry_heap_.top();
            auto now = std::chrono::steady_clock::now();

            if (now >= next.expire_time)
            {
                expiry_heap_.pop();
                auto it = expiry_map_.find(next.key);

                if (it != expiry_map_.end() && it->second <= now)
                {
                    expiry_map_.erase(it);
                }
                // Continuing to check next expired keys immediately ?
            }
            else
            {
                cv_.wait_until(lock, next.expire_time);
            }
        }
    }
}