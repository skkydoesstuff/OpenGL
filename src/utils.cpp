#include "utils.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif

std::string readFileIntoString(std::string filepath) {
	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filepath << std::endl;
	}

	std::string fileContent(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

	file.close();
	
	return fileContent;
}


std::string getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::path(path).parent_path().string();

#elif __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return std::filesystem::path(path).parent_path().string();
    else
        return {}; // buffer too small (shouldn't happen with 1024)

#elif __linux__
    char path[1024];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (count != -1) {
        path[count] = '\0';
        return std::filesystem::path(path).parent_path().string();
    } else {
        return {};
    }
#else
    return {}; // unsupported platform
#endif
}
