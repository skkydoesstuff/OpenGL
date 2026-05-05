CXX      := g++
CC       := gcc
TARGET   := build\app.exe

CXXFLAGS := -O0 -g -Wall -Wextra -std=c++20
CFLAGS   := -O0 -g

CXXFLAGS_RELEASE := -O3 -DNDEBUG -Wall -Wextra -std=c++20
CFLAGS_RELEASE   := -O3 -DNDEBUG

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
APP_OBJ_DIR := $(OBJ_DIR)/app
VENDOR_OBJ_DIR := $(OBJ_DIR)/vendor

ASSETS_SRC := assets
ASSETS_DST := $(BUILD_DIR)/assets

SHELL := cmd.exe

# -------------------------
# SOURCES
# -------------------------
APP_SOURCES := \
    src/main.cpp \
    src/core/scene/scene.cpp \
    src/core/scene/objects/light.cpp \
    src/core/scene/objects/model.cpp \
    src/core/scene/objects/camera.cpp \
    src/core/scene/loaders/loadShaders.cpp \
    src/core/scene/loaders/loadMaterials.cpp \
    src/core/scene/loaders/loadTextures.cpp \
    src/core/scene/loaders/loadMeshes.cpp \
    src/core/scene/loaders/loadModels.cpp \
    src/core/scene/loaders/loadLights.cpp \
    src/core/scene/loaders/loadPasses.cpp \
    src/core/scene/savers/saveShaders.cpp \
    src/core/scene/savers/saveMaterials.cpp \
    src/core/scene/savers/saveTextures.cpp \
    src/core/scene/savers/saveMeshes.cpp \
    src/core/scene/savers/saveModels.cpp \
    src/core/scene/savers/saveLights.cpp \
    src/core/scene/savers/savePasses.cpp \
    src/core/renderer/mesh.cpp \
    src/core/renderer/renderer.cpp \
    src/core/renderer/renderTarget.cpp \
    src/core/renderer/shader.cpp \
    src/core/renderer/texture.cpp \
    src/core/renderer/material.cpp \
    src/app/init.cpp \
    src/app/setup.cpp \
    src/app/run.cpp \
    src/app/mainLoop.cpp \
    src/app/logic.cpp \
    src/app/imguiRender.cpp \
    src/app/render.cpp \
    src/app/cleanup.cpp \
    src/utils/fileUtils.cpp \
    src/utils/objLoader.cpp \
    src/utils/mtlLoader.cpp \
    src/utils/tiny_obj_loader.cpp \
    src/utils/jsonHelpers.cpp \
    src/utils/stb_image.cpp 

IMGUI_SOURCES := \
    external/imgui/imgui.cpp \
    external/imgui/imgui_draw.cpp \
    external/imgui/imgui_tables.cpp \
    external/imgui/imgui_widgets.cpp \
    external/imgui/backends/imgui_impl_glfw.cpp \
    external/imgui/backends/imgui_impl_opengl3.cpp

GLAD_SOURCES := \
    external/glad/src/glad.c

# -------------------------
# OBJECTS
# -------------------------

APP_OBJS := $(APP_SOURCES:.cpp=.o)
APP_OBJS := $(subst src,$(APP_OBJ_DIR),$(APP_OBJS))
IMGUI_OBJS := $(IMGUI_SOURCES:.cpp=.o)
IMGUI_OBJS := $(subst external/imgui,$(VENDOR_OBJ_DIR),$(IMGUI_OBJS))
GLAD_OBJS := $(GLAD_SOURCES:.c=.o)
GLAD_OBJS := $(subst external/glad/src,$(VENDOR_OBJ_DIR),$(GLAD_OBJS))

OBJ := $(APP_OBJS) $(IMGUI_OBJS) $(GLAD_OBJS)

# -------------------------
# INCLUDES
# -------------------------
INCLUDES := \
    -Iexternal/imgui \
    -Iexternal/glfw/include \
    -Iexternal/imgui/backends \
    -Iexternal/glad/include \
    -Iexternal/glm \
    -Iexternal/nlohmann \
    -Isrc

# -------------------------
# LIBS
# -------------------------
LIBS := -Lexternal/glfw/lib -lglfw3 -lopengl32 -lgdi32 -lshlwapi

# -------------------------
# CREATE DIRECTORIES
# -------------------------
$(shell if not exist "$(subst /,\,$(APP_OBJ_DIR))" mkdir "$(subst /,\,$(APP_OBJ_DIR))")
$(shell if not exist "$(subst /,\,$(VENDOR_OBJ_DIR))" mkdir "$(subst /,\,$(VENDOR_OBJ_DIR))")
$(shell if not exist "$(subst /,\,$(APP_OBJ_DIR))\utils" mkdir "$(subst /,\,$(APP_OBJ_DIR))\utils")
$(shell if not exist "$(subst /,\,$(APP_OBJ_DIR))\core\scene" mkdir "$(subst /,\,$(APP_OBJ_DIR))\core\scene")
$(shell if not exist "$(subst /,\,$(APP_OBJ_DIR))\core\renderer" mkdir "$(subst /,\,$(APP_OBJ_DIR))\core\renderer")
$(shell if not exist "$(subst /,\,$(APP_OBJ_DIR))\app" mkdir "$(subst /,\,$(APP_OBJ_DIR))\app")

define make-dir
if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
endef

# -------------------------
# BUILD
# -------------------------
.PHONY: all clean

all: $(TARGET)

copy_assets:
	xcopy /E /I /Y "$(ASSETS_SRC)" "$(ASSETS_DST)" >nul

.PHONY: copy_assets

all: copy_assets $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

# -------------------------
# APP RULES
# -------------------------
$(APP_OBJ_DIR)/%.o: src/%.cpp
	$(make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# VENDOR RULES (ImGui + GLAD)
# -------------------------
$(VENDOR_OBJ_DIR)/%.o: external/imgui/%.cpp
	$(make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(VENDOR_OBJ_DIR)/%.o: external/imgui/backends/%.cpp
	$(make-dir)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(VENDOR_OBJ_DIR)/%.o: external/glad/src/%.c
	$(make-dir)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# -------------------------
# CLEAN (ONLY APP + BIN, NOT VENDOR)
# -------------------------
BUILD_DIR_WIN := $(subst /,\,$(BUILD_DIR))

clean:
	if exist "$(subst /,\,$(APP_OBJ_DIR))" rmdir /S /Q "$(subst /,\,$(APP_OBJ_DIR))"
	if exist $(TARGET) del /Q $(TARGET)

release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: CFLAGS   := $(CFLAGS_RELEASE)
release: clean all