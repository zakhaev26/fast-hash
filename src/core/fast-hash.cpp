#include "core/fast-hash.hpp"

void FastHash::set(const std::string &key, const std::string &value)
{
    store[key] = value;
}

bool FastHash::get(const std::string &key, std::string &value) const
{
    auto it = store.find(key);

    if (it != store.end())
    {
        value = it->second;
        return true;
    }

    return false;
}

bool FastHash::del(const std::string &key)
{
    return store.erase(key) > 0;
}