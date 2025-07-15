// Copyright 2025 Soubhik Gon
#include "core/fast-hash.hpp"
#include "core/fast-hash.hpp"
#include "core/command.hpp"
#include "core/parser.hpp"

using json = nlohmann::json;

FastHash::FastHash(const std::string &aof_path, AOFSyncPolicy policy) : aof_logger_(aof_path, policy), is_aof_loading_(false)
{
  this->ttl_manager_.set_expire_callback([this](const std::string &key)
                                         {
                                           std::lock_guard<std::mutex> lock(this->mutex_);
                                           this->store_.erase(key);
                                           // std::cout << "[DEBUG]: key '" << key << "' also removed from store_\n";
                                         });
}

FastHash::~FastHash() { this->stop(); }

void FastHash::stop() { this->ttl_manager_.stop(); }

bool FastHash::set(const std::string &key, const std::string &value,
                   std::optional<int> ttl_seconds)
{
  std::lock_guard<std::mutex> lock(this->mutex_);
  this->store_[key] = Value{value};

  if (ttl_seconds.has_value())
  {
    auto expire_time = std::chrono::steady_clock::now() +
                       std::chrono::seconds(ttl_seconds.value());
    this->ttl_manager_.add_expiration(key, expire_time);

    if (!is_aof_loading_)
      aof_logger_.log("SETEX " + key + " " + std::to_string(ttl_seconds.value()) + " " + value);
  }
  else
  {
    this->ttl_manager_.remove_expiration(key);
    if (!is_aof_loading_)
      aof_logger_.log("SET " + key + " " + value);
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
    // std::cout << "[DEBUG]: key expired already!\n";
    store_.erase(key);
    this->ttl_manager_.remove_expiration(key);
    return std::nullopt;
  }
  // std::cout << "[DEBUG]: key not expired !\n";

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
  if (erased && !is_aof_loading_)
    aof_logger_.log("DEL " + key);
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

  if (!is_aof_loading_)
    aof_logger_.log("EXPIRE " + key + " " + std::to_string(seconds));

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

  if (!is_aof_loading_)
    aof_logger_.log("PERSIST " + key);
  return true;
}

void FastHash::flush_all()
{
  std::lock_guard<std::mutex> lock(mutex_);
  store_.clear();
  ttl_manager_.clear_all();

  if (!is_aof_loading_)
    aof_logger_.log("FLUSHALL");
}

bool FastHash::save(const std::string &filepath) const
{
  std::lock_guard<std::mutex> lock(mutex_);
  nlohmann::json data = this->serialize();
  // std::cout << data << std::endl;
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
    // std::cout << key << " " << value << std::endl;
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

// void FastHash::enable_aof(const std::string &path, AOFSyncPolicy policy)
// {
//   aof_log_ = std::make_unique<AOFLogger>(path, policy);
// }

/*
bool FastHash::load_aof(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "[WARN] No AOF file found at: " << filepath << "\n";
        return false;
    }

    std::string line;
    bool original_flag = ao;  // Save current state
    this->ao = false;               // Disable logging during replay

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        Command cmd = Command::parse(line);
        if (cmd.type == Command::INVALID)
        {
            std::cerr << "[WARN] Invalid AOF line: " << line << "\n";
            continue;
        }

        cmd.execute(*this);
    }

    this->aof_enabled_ = original_flag;
    return true;
}

// bool FastHash::load_aof(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "[WARN] No AOF file found at: " << filepath << "\n";
    return false;
  }

  std::string line;
  bool original_flag = this->aof_enabled_;
  this->aof_enabled_ = false;

  while (std::getline(file, line))
  {
    if (line.empty())
      continue;

    Command cmd = Command::parse(line);
    if (cmd.type == Command::INVALID)
    {
      std::cerr << "[WARN] Invalid AOF line: " << line << "\n";
      continue;
    }

    cmd.execute(*this);
  }

  this->aof_enabled_ = original_flag;
  return true;
}
*/

void FastHash::replayAOF(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "[WARN] AOF file not found: " << filepath << "\n";
    return;
  }

  is_aof_loading_ = true;
  // std::cout << "is_aof_loading_=" << is_aof_loading_ << std::endl;

  std::string line;
  int count = 0;

  while (std::getline(file, line))
  {
    auto tokens = parser::tokenize(line);
    if (tokens.empty())
      continue;

    std::string cmd = parser::to_upper(tokens[0]);

    if (cmd == "SET" && tokens.size() == 3)
    {
      this->set(tokens[1], tokens[2]);
    }
    else if (cmd == "SETEX" && tokens.size() == 4)
    {
      try
      {
        int ttl = std::stoi(tokens[2]);
        this->set(tokens[1], tokens[3], ttl);
      }
      catch (...)
      {
        std::cerr << "[ERROR] Invalid TTL in AOF SETEX\n";
      }
    }
    else if (cmd == "DEL" && tokens.size() == 2)
    {
      this->del(tokens[1]);
    }
    else if (cmd == "EXPIRE" && tokens.size() == 3)
    {
      try
      {
        int ttl = std::stoi(tokens[2]);
        this->expire(tokens[1], ttl);
      }
      catch (...)
      {
        std::cerr << "[ERROR] Invalid TTL in AOF EXPIRE\n";
      }
    }
    else if (cmd == "PERSIST" && tokens.size() == 2)
    {
      this->persist(tokens[1]);
    }
    else if (cmd == "FLUSHALL")
    {
      this->flush_all();
    }
    else
    {
      std::cerr << "[WARN] Unrecognized or invalid AOF command: " << line << "\n";
    }

    count++;
  }

  is_aof_loading_ = false;
  // std::cout << "is_aof_loading_=" << is_aof_loading_ << std::endl;

  std::cout
      << "[INFO] AOF replayed " << count << " commands\n";
}
