cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)

# --- GLAD ---
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- SDL3 ---
add_library(SDL3 STATIC IMPORTED)  # or SHARED if you want dynamic linking
set_target_properties(SDL3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/SDL3/lib/libSDL3.dll.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/SDL3/include"
)

# --- Collect all .c source files under src ---
# IMPORTANT: CMake won't detect new files automatically unless reconfigured
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")

# --- Executable ---
add_executable(app ${SRC_FILES})

# --- Link libraries ---
target_link_libraries(app
    PRIVATE SDL3 glad
)

# --- OpenGL (needed on Windows + MinGW) ---
find_package(OpenGL REQUIRED)
target_link_libraries(app PRIVATE OpenGL::GL)

# --- Optional: show what files are included ---
message(STATUS "Source files:")
foreach(f ${SRC_FILES})
    message(STATUS "  ${f}")
endforeach()
