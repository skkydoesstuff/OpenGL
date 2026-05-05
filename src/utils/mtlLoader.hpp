#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "core/renderer/material.hpp"

void saveMTL(const std::string& path, std::unordered_map<std::string, std::shared_ptr<Material>> materials);

class MTLLoader {
public:
    static std::unordered_map<std::string, std::shared_ptr<Material>>
    load(const std::string& path);
};
