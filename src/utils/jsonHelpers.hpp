#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>

void printJson(const json& j, int indent = 0);
json parseJson(const std::string& path);