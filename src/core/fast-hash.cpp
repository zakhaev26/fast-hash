// Copyright 2025 Soubhik Gon
#include "core/fast-hash.hpp"
using json = nlohmann::json;

FastHash::FastHash()
{
  this->ttl_manager_.set_expire_callback([this](const std::string &key)
                                         {
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->store_.erase(key);
    std::cout << "[DEBUG]: key '" << key << "' also removed from store_\n"; });
}

FastHash::~FastHash() { this->stop(); }

void FastHash::stop() { this->ttl_manager_.stop(); }

bool FastHash::set(const std::string &key, const std::string &value,
                   std::optional<int> ttl_seconds)
{
  std::lock_guard<std::mutex> lock(this->mutex_);
  this->store_[key] = Value{value};
  // std::cout << "[DEBUG]: key " << key << " value " << value << " ttl_seconds
  // " << ttl_seconds.value() << "\n";
  if (ttl_seconds.has_value())
  {
    auto expire_time = std::chrono::steady_clock::now() +
                       std::chrono::seconds(ttl_seconds.value());
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     expire_time.time_since_epoch())
                     .count()
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

  auto expire_time =
      std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
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
  auto remaining =
      std::chrono::duration_cast<std::chrono::seconds>(expiry_opt.value() - now)
          .count();

  return remaining > 0 ? static_cast<int>(remaining) : -2;
}

std::vector<std::string> FastHash::keys(const std::string &pattern)
{
  // return std::vector<std::string>();
  std::lock_guard<std::mutex> lock(this->mutex_);
  std::vector<std::string> response;

  std::string regex_pattern =
      std::regex_replace(pattern, std::regex(R"([\.\^\$\+\(\)\[\]\{\}])"),
                         R"(\$&)"); // escape regex
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

void FastHash::flush_all()
{
  std::lock_guard<std::mutex> lock(mutex_);
  store_.clear();
  ttl_manager_.clear_all();
}

bool FastHash::save(const std::string &filepath) const
{
  std::lock_guard<std::mutex> lock(mutex_);
  nlohmann::json data = this->serialize();
  std::cout << data << std::endl;
  std::ofstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] Failed to open file for SAVE\n";
    return false;
  }

  file << data.dump(2);
  return true;
}

bool FastHash::save_async(const std::string &filepath) const
{
  std::thread([this, filepath]()
              {
        nlohmann::json data;
        {
            std::lock_guard<std::mutex> lock(this->mutex_);
            data = this->serialize();
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Failed to open file for ASAVE\n";
            return;
        }

        file << data.dump(2); })
      .detach();

  return true;
}

json FastHash::serialize() const
{
  nlohmann::json j;

  auto now = std::chrono::steady_clock::now();

  for (const auto &[key, val] : store_)
  {
    nlohmann::json entry;
    entry["value"] = val.data;

    auto expiry_time = ttl_manager_.get_expiry_time(key);
    if (expiry_time.has_value())
    {
      auto ttl = std::chrono::duration_cast<std::chrono::seconds>(expiry_time.value() - now).count();
      if (ttl > 0)
      {
        entry["ttl"] = ttl;
      }
    }

    j[key] = entry;
  }

  return j;
}

bool FastHash::load(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] Failed to open file for LOAD\n";
    return false;
  }

  nlohmann::json data;
  try
  {
    file >> data;
  }
  catch (const std::exception &e)
  {
    std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << "\n";
    return false;
  }

  std::lock_guard<std::mutex> lock(mutex_);

  store_.clear();
  ttl_manager_.clear_all();

  auto now = std::chrono::steady_clock::now();

  for (auto it = data.begin(); it != data.end(); ++it)
  {
    std::string key = it.key();
    auto &entry = it.value();

    if (!entry.contains("value") || !entry["value"].is_string())
    {
      std::cerr << "[ERROR] Invalid entry for key '" << key << "', missing value\n";
      continue;
    }

    std::string value = entry["value"];
    std::cout << key << " "<< value << std::endl; 
    store_[key] = Value{value};

    if (entry.contains("ttl") && entry["ttl"].is_number_integer())
    {
      int ttl_seconds = entry["ttl"];
      if (ttl_seconds > 0)
      {
        auto expire_time = now + std::chrono::seconds(ttl_seconds);
        ttl_manager_.add_expiration(key, expire_time);
      }
    }
  }

  return true;
}
