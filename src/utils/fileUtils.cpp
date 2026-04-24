#include "utils/fileUtils.hpp"
#include <windows.h>
#include <shlwapi.h>
#include <string>
#include <fstream>
#include <sstream>

std::string getExecutableDirectory() {
    char buffer[MAX_PATH];

    DWORD len = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (len == 0) return "";

    PathRemoveFileSpecA(buffer);
    return std::string(buffer);
}

std::string readFile(const std::string& path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    return buffer.str();
}