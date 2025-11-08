# Configuration
build_dir := "build"
output := build_dir / "app"
cc := "g++"
cflags := "-g -Wall -std=c++17"
includes := "-Iinclude -Ithird_party/glad/include -Ithird_party/glm"
libs := "-lglfw -ldl -lm -lGL"

# Default target
default: build

# Build everything
build: setup compile link copy_assets
    echo "✓ Build complete: {{output}}"

# Create build dir
setup:
    mkdir -p {{build_dir}}

# Compile sources incrementally
compile:
    #!/usr/bin/env bash
    set -euo pipefail
    sources=$(find src third_party -name "*.cpp" -o -name "*.c")
    for src in $sources; do
        obj="{{build_dir}}/$(basename $src).o"
        if [ ! -f $obj ] || [ $src -nt $obj ]; then
            echo "Compiling $src"
            {{cc}} -c "$src" {{cflags}} {{includes}} -o "$obj"
        fi
    done

# Link objects
link:
    #!/usr/bin/env bash
    set -euo pipefail
    objs=""
    sources=$(find src third_party -name "*.cpp" -o -name "*.c")
    for src in $sources; do
        objs="$objs {{build_dir}}/$(basename $src).o"
    done
    echo "Linking..."
    {{cc}} $objs -o {{output}} {{libs}}

# Copy assets
copy_assets:
    rm -rf {{build_dir}}/assets
    cp -r assets {{build_dir}}/assets

# Clean
clean:
    rm -rf {{build_dir}}

# Rebuild
rebuild: clean build

# Run
run: build
    ./{{output}}