#include "utils/fileUtils.hpp"
#include "utils/debugUtils.hpp"

#include <windows.h>
#include <shlwapi.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

std::string getExecutableDirectory() {
    char buffer[MAX_PATH];

    DWORD len = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (len == 0) return "";

    PathRemoveFileSpecA(buffer);
    return std::string(buffer);
}

std::string readFile(const std::string& path) {
    std::ifstream t(path);
    if (!t.is_open()) {
        DEBUG_PRINT(std::cout << "Failed to open file: " << path << "\n");
        return "";
    }
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}