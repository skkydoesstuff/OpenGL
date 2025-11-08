#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

inline std::string getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::path(path).parent_path().string();
#elif __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return std::filesystem::path(path).parent_path().string();
    else {
        std::string buf(size, '\0');
        _NSGetExecutablePath(buf.data(), &size);
        return std::filesystem::path(buf).parent_path().string();
    }
#elif __linux__
    char path[1024];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::filesystem::path(path).parent_path().string();
    }
    return "";
#else
    #error "Unsupported platform"
#endif
}