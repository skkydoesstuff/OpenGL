CXX := g++
CC  := gcc

TARGET    := build/app.exe
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj

SHELL := cmd.exe

# -------------------------
# FLAGS
# -------------------------
CXXFLAGS := -O0 -g -Wall -Wextra -std=c++20
CFLAGS   := -O0 -g

CXXFLAGS_RELEASE := -O3 -DNDEBUG -Wall -Wextra -std=c++20
CFLAGS_RELEASE   := -O3 -DNDEBUG

# -------------------------
# SOURCES
# -------------------------
rwildcard = $(foreach d, $(wildcard $1*), \
                $(call rwildcard, $d/, $2) \
                $(filter $(subst *, %, $2), $d))

APP_SOURCES := $(call rwildcard, src/, *.cpp)
IMGUI_SOURCES := $(wildcard external/imgui/*.cpp) \
                 $(wildcard external/imgui/backends/imgui_impl_glfw.cpp) \
                 $(wildcard external/imgui/backends/imgui_impl_opengl3.cpp)
GLAD_SOURCES  := $(wildcard external/glad/src/*.c)

# -------------------------
# OBJECTS
# -------------------------
APP_OBJS   := $(patsubst src/%,    $(OBJ_DIR)/app/%,    $(APP_SOURCES:.cpp=.o))
IMGUI_OBJS := $(patsubst external/imgui/%, $(OBJ_DIR)/vendor/%, $(IMGUI_SOURCES:.cpp=.o))
GLAD_OBJS  := $(patsubst external/glad/src/%, $(OBJ_DIR)/vendor/%, $(GLAD_SOURCES:.c=.o))

OBJ := $(APP_OBJS) $(IMGUI_OBJS) $(GLAD_OBJS)

# -------------------------
# INCLUDES
# -------------------------
INCLUDES := \
    -Iexternal/imgui \
    -Iexternal/imgui/backends \
    -Iexternal/glfw/include \
    -Iexternal/glad/include \
    -Iexternal/glm \
    -Iexternal/nlohmann \
    -Isrc

# -------------------------
# LIBS
# -------------------------
LIBS := -Lexternal/glfw/lib -lglfw3 -lopengl32 -lgdi32 -lshlwapi

# -------------------------
# ASSETS
# -------------------------
ASSETS_SRC := assets
ASSETS_DST := $(BUILD_DIR)/assets

# -------------------------
# BUILD
# -------------------------
.PHONY: all clean release copy_assets

all: copy_assets $(TARGET)

release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: CFLAGS   := $(CFLAGS_RELEASE)
release: clean all

copy_assets:
	xcopy /E /I /Y "$(ASSETS_SRC)" "$(ASSETS_DST)" >nul

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

# -------------------------
# APP RULES
# -------------------------
$(OBJ_DIR)/app/%.o: src/%.cpp
	$(call make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# VENDOR RULES
# -------------------------
$(OBJ_DIR)/vendor/%.o: external/imgui/%.cpp
	$(call make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/vendor/%.o: external/imgui/backends/%.cpp
	$(call make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/vendor/%.o: external/glad/src/%.c
	$(call make-dir)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# HELPERS
# -------------------------
define make-dir
if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
endef

# -------------------------
# CLEAN
# -------------------------
clean:
	if exist "$(subst /,\,$(OBJ_DIR)/app)" rmdir /S /Q "$(subst /,\,$(OBJ_DIR)/app)"
	if exist "$(subst /,\,$(TARGET))" del /Q "$(subst /,\,$(TARGET))"