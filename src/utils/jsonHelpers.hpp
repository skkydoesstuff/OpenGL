#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <string>

json parseJson(const std::string& path);