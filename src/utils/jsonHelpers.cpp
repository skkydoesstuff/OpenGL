#include "utils/jsonHelpers.hpp"
#include "utils/fileUtils.hpp"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <fstream>

void printJson(const json& j, int indent = 0) {
    std::string pad(indent, ' ');

    if (j.is_object()) {
        for (auto& [key, value] : j.items()) {
            std::cout << pad << key << ": ";

            if (value.is_object() || value.is_array()) {
                std::cout << "\n";
                printJson(value, indent + 2);
            } else {
                std::cout << value << "\n";
            }
        }
    }

    else if (j.is_array()) {
        std::cout << pad << "[\n";

        for (size_t i = 0; i < j.size(); i++) {
            std::cout << pad << "  - (" << i << ")\n";
            printJson(j[i], indent + 4);
        }

        std::cout << pad << "]\n";
    }

    else {
        std::cout << pad << j << "\n";
    }
}

json parseJson(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open JSON: " << path << "\n";
        return {};
    }

    json j;
    file >> j;

    printJson(j);

    return j;
}