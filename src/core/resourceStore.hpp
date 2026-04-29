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

    std::vector<T*> values() {
        std::vector<T*> out;
        out.reserve(resources.size());

        for (auto& [k, v] : resources)
            out.push_back(v.get());

        return out;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> resources;
};

template<typename T>
class UniqueResourceStore {
public:
    template<typename... Args>
    T* create(const std::string& key, Args&&... args) {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        resources.emplace(key, std::move(obj));
        return ptr;
    }

    T* get(const std::string& key) {
        auto it = resources.find(key);
        return (it == resources.end()) ? nullptr : it->second.get();
    }

    const T* get(const std::string& key) const {
        auto it = resources.find(key);
        return (it == resources.end()) ? nullptr : it->second.get();
    }

    bool exists(const std::string& key) const {
        return resources.find(key) != resources.end();
    }

    void remove(const std::string& key) {
        resources.erase(key);
    }

    std::vector<T*> values() {
        std::vector<T*> out;
        out.reserve(resources.size());

        for (auto& [k, v] : resources)
            out.push_back(v.get());

        return out;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<T>> resources;
};