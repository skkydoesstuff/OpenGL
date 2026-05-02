#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "core/renderer/material.hpp"

void saveMTL(const std::string& path, std::unordered_map<std::string, Material*> materials);

class MTLLoader {
public:
    static std::unordered_map<std::string, Material*>
    load(const std::string& path);
};