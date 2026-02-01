cmake_minimum_required(VERSION 3.16)
project(app LANGUAGES C CXX)  # Add C language support
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)

# --- GLAD (C code) ---
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC 
    external/glad/include
    external/glm
    external/stb_image
    src/engine
    src/game
)
# Remove this line - it's causing the issue
# set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

# --- SDL3 ---
add_library(SDL3 STATIC IMPORTED)
set_target_properties(SDL3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/SDL3/lib/libSDL3.dll.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/SDL3/include"
)

# --- Collect all .cpp source files under src ---
file(GLOB_RECURSE ENGINE_SRC_FILES "${CMAKE_SOURCE_DIR}/src/engine/*.cpp")
file(GLOB_RECURSE GAME_SRC_FILES "${CMAKE_SOURCE_DIR}/src/game/*.cpp")

# --- Executable ---
add_executable(app 
    ${ENGINE_SRC_FILES}
    ${GAME_SRC_FILES}
)

# --- Link libraries ---
target_link_libraries(app
    PRIVATE SDL3 glad
)

# --- OpenGL (needed on Windows + MinGW) ---
find_package(OpenGL REQUIRED)
target_link_libraries(app PRIVATE OpenGL::GL)

# --- Optional: show what files are included ---
message(STATUS "Source files:")
foreach(f ${ENGINE_SRC_FILES} ${GAME_SRC_FILES})
    message(STATUS "  ${f}")
endforeach()