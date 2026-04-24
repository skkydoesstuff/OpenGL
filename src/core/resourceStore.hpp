#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <utility>

template<typename T>
class ResourceStore {
public:
    std::shared_ptr<T> get(const std::string& key) {
        auto it = resources.find(key);
        if (it == resources.end())
            return nullptr;

        return it->second;
    }

    template<typename... Args>
    std::shared_ptr<T> create(const std::string& key, Args&&... args) {
        auto res = std::make_shared<T>(std::forward<Args>(args)...);
        resources[key] = res;
        return res;
    }

    bool exists(const std::string& key) const {
        return resources.find(key) != resources.end();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> resources;
};