CXX = g++
CXXFLAGS = -Iinclude -Iexternal/glad/include -std=c++17 -Wall -Wextra -g
LDFLAGS_LINUX = -lglfw -ldl -lGL -lX11 -lpthread -lXrandr -lXi
LDFLAGS_WINDOWS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32

SRC = $(wildcard src/*.cpp) external/glad/src/glad.c
OBJ = $(SRC:src/%.cpp=build/%.o)
OBJ := $(OBJ:external/glad/src/%.c=build/%.o)
TARGET = build/app

# Detect OS (Linux default)
ifeq ($(OS),Windows_NT)
	LDFLAGS = $(LDFLAGS_WINDOWS)
	EXE = .exe
else
	LDFLAGS = $(LDFLAGS_LINUX)
	EXE =
endif

all: $(TARGET)$(EXE) copy_assets

$(TARGET)$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: external/glad/src/%.c | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

copy_assets:
	@if [ -d build/assets ]; then rm -rf build/assets; fi
	@cp -r assets build/

clean:
	rm -rf build

# Windows build target (using MinGW)
win:
	$(MAKE) OS=Windows_NT \
	CXX=x86_64-w64-mingw32-g++ \
	TARGET=build/app \
	LDFLAGS="-static -static-libgcc -static-libstdc++ -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32"
