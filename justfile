# Configuration
build_dir := "build"
output := build_dir / "app"
cc := "g++"
cflags := "-g -Wall -std=c++17"
includes := "-Iinclude -Ithird_party/glad/include"
libs := "-lglfw -ldl -lm -lGL"

# Default recipe
default: build

# Build everything
build: setup compile link copy_assets
    @echo "✓ Build complete: {{output}}"

# Create build directory
setup:
    @mkdir -p {{build_dir}}

# Find and compile all .cpp and .c files
compile: setup
    #!/usr/bin/env bash
    set -euo pipefail
    # Compile C++ files
    for src in $(find src third_party -name "*.cpp" 2>/dev/null); do
        obj="{{build_dir}}/$(basename "$src" .cpp).o"
        echo "Compiling: $src"
        {{cc}} -c "$src" {{cflags}} {{includes}} -o "$obj"
    done
    # Compile C files
    for src in $(find src third_party -name "*.c" 2>/dev/null); do
        obj="{{build_dir}}/$(basename "$src" .c).o"
        echo "Compiling: $src"
        {{cc}} -c "$src" {{cflags}} {{includes}} -o "$obj"
    done

# Link all object files
link: compile
    @echo "Linking..."
    @{{cc}} {{build_dir}}/*.o -o {{output}} {{libs}}

copy_assets:
  @echo "Copying assets..."
  rm -rf build/assets
  cp -r assets build/assets

# Clean build artifacts
clean:
    rm -rf {{build_dir}}

# Rebuild from scratch
rebuild: clean build

# Run the executable
run: build
    ./{{output}}

# List all source files
list-sources:
    @find src third_party -name "*.cpp" 2>/dev/null || echo "No source files found"

# Show build configuration
show-config:
    @echo "Build Directory: {{build_dir}}"
    @echo "Output: {{output}}"
    @echo "Compiler: {{cc}}"
    @echo "Includes: {{includes}}"
    @echo "Libraries: {{libs}}"