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
        std::lock_guard<std::mutex> lock(this->mutex_);
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
        std::cout << "DEBUG: key '" << key << "' not found in expiry_map_ → not expired\n";
        return false;
    }

    auto expire_time = it->second;
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto expire_ms = std::chrono::duration_cast<std::chrono::milliseconds>(expire_time.time_since_epoch()).count();

    std::cout << "DEBUG: checking key '" << key << "'\n";
    std::cout << "DEBUG: now_ms = " << now_ms << ", expire_ms = " << expire_ms << "\n";

    if (now >= expire_time)
    {
        std::cout << "DEBUG: key '" << key << "' is expired → erasing from map\n";
        expiry_map_.erase(it);
        return true;
    }

    std::cout << "DEBUG: key '" << key << "' has not yet expired\n";
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

                    if (on_expire_callback_)
                    {
                        lock.unlock();
                        on_expire_callback_(next.key);
                        lock.lock();
                    }

                    std::cout << "[SYSTEM]: Sweeped 1 key in background thread." << next.key << "\n";
                }
            }
            else
            {
                cv_.wait_until(lock, next.expire_time);
            }
        }
    }
}

void TTLManager::set_expire_callback(std::function<void(const std::string &)> cb)
{
    this->on_expire_callback_ = cb;
}

std::optional<std::chrono::steady_clock::time_point> TTLManager::get_expiry_time(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = expiry_map_.find(key);
    if (it != expiry_map_.end())
    {
        return it->second;
    }
    return std::nullopt;
}

bool TTLManager::has_expiration(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return expiry_map_.find(key) != expiry_map_.end();
}
