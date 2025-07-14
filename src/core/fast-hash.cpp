#include <iostream>
#include <regex>
#include "core/fast-hash.hpp"
#include "core/fast-hash.hpp"

FastHash::FastHash()
{
    this->ttl_manager_.set_expire_callback(
        [this](const std::string &key)
        {
            std::lock_guard<std::mutex> lock(this->mutex_);
            this->store_.erase(key);
            std::cout << "[DEBUG]: key '" << key << "' also removed from store_\n";
        });
}

FastHash::~FastHash()
{
    this->stop();
}

void FastHash::stop()
{
    this->ttl_manager_.stop();
}

bool FastHash::set(
    const std::string &key,
    const std::string &value,
    std::optional<int> ttl_seconds)
{
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->store_[key] = Value{value};
    // std::cout << "[DEBUG]: key " << key << " value " << value << " ttl_seconds " << ttl_seconds.value() << "\n";
    if (ttl_seconds.has_value())
    {
        auto expire_time = std::chrono::steady_clock::now() + std::chrono::seconds(ttl_seconds.value());
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(expire_time.time_since_epoch()).count()
                  << "\n";
        this->ttl_manager_.add_expiration(key, expire_time);
    }
    else
    {
        this->ttl_manager_.remove_expiration(key);
    }

    return true;
}

// void FastHash::set(const std::string &key, const std::string &value)
// {
//     store[key] = value;
// }

std::optional<std::string> FastHash::get(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);

    if (this->ttl_manager_.expired(key))
    {
        std::cout << "[DEBUG]: key expired already!\n";
        store_.erase(key);
        this->ttl_manager_.remove_expiration(key);
        return std::nullopt;
    }
    std::cout << "[DEBUG]: key not expired !\n";

    auto it = store_.find(key);
    if (it == store_.end())
    {
        return std::nullopt;
    }
    return it->second.data;
}

// bool FastHash::get(const std::string &key, std::string &value) const
// {
//     auto it = store.find(key);

//     if (it != store.end())
//     {
//         value = it->second;
//         return true;
//     }

//     return false;
// }

bool FastHash::del(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);
    bool erased = store_.erase(key) > 0;
    this->ttl_manager_.remove_expiration(key);
    return erased;
}

bool FastHash::expire(const std::string &key, int seconds)
{
    if (seconds <= 0)
        return false;
    std::lock_guard<std::mutex> lock(this->mutex_);
    if (store_.find(key) == store_.end())
        return false;

    auto expire_time = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    this->ttl_manager_.add_expiration(key, expire_time);
    return true;
}

int FastHash::ttl(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);

    auto it = store_.find(key);
    if (it == store_.end())
        return -2;

    // safety
    if (ttl_manager_.expired(key))
    {
        store_.erase(it);
        ttl_manager_.remove_expiration(key);
        return -2;
    }

    auto expiry_opt = ttl_manager_.get_expiry_time(key);
    if (!expiry_opt.has_value())
    {
        return -1;
    }

    auto now = std::chrono::steady_clock::now();
    auto remaining = std::chrono::duration_cast<std::chrono::seconds>(expiry_opt.value() - now).count();

    return remaining > 0 ? static_cast<int>(remaining) : -2;
}

std::vector<std::string> FastHash::keys(const std::string &pattern)
{
    // return std::vector<std::string>();
    std::lock_guard<std::mutex> lock(this->mutex_);
    std::vector<std::string> response;

    std::string regex_pattern = std::regex_replace(pattern, std::regex(R"([\.\^\$\+\(\)\[\]\{\}])"), R"(\$&)"); // escape regex
    regex_pattern = std::regex_replace(regex_pattern, std::regex(R"(\*)"), ".*");
    regex_pattern = std::regex_replace(regex_pattern, std::regex(R"(\?)"), ".");

    std::regex pattern_regex("^" + regex_pattern + "$");

    for (const auto &pair : store_)
    {
        const std::string &key = pair.first;

        if (this->ttl_manager_.expired(key))
        {
            store_.erase(key);
            this->ttl_manager_.remove_expiration(key);
            continue;
        }

        if (std::regex_match(key, pattern_regex))
        {
            response.emplace_back(key);
        }
    }

    return response;
}

bool FastHash::exists(const std::string &key)
{
    std::lock_guard<std::mutex> lock(this->mutex_);
    if (this->ttl_manager_.expired(key))
    {
        store_.erase(key);
        ttl_manager_.remove_expiration(key);
        return false;
    }

    return store_.find(key) != store_.end();
}

bool FastHash::persist(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (ttl_manager_.expired(key))
    {
        store_.erase(key);
        ttl_manager_.remove_expiration(key);
        return false;
    }

    auto it = store_.find(key);
    if (it == store_.end())
        return false;

    if (!ttl_manager_.has_expiration(key))
        return false;

    ttl_manager_.remove_expiration(key);
    return true;
}
