#pragma once

#include <string>
#include <unordered_map>

#include "core/renderer/material.hpp"

class MTLLoader {
public:
    static std::unordered_map<std::string, Material>
    load(const std::string& path);
};