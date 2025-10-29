## Features

- OpenGL 3.3+ rendering
- GLFW window management
- GLAD OpenGL loader
- STB image loading support
- Meson build system
- C++17 standard

## Prerequisites

Before building, ensure you have the following installed:

- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
- Meson build system (0.55.0 or later)
- Ninja build tool
- GLFW3 development libraries
- OpenGL development libraries

### Windows

**Option 1: MSYS2 (Recommended)**

```bash
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 MinGW64 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-meson mingw-w64-x86_64-ninja mingw-w64-x86_64-glfw
```

**Option 2: Visual Studio**

1. Install Visual Studio 2017 or later with C++ support
2. Install Python 3.7+ from https://www.python.org/
3. Install Meson and Ninja:
   ```cmd
   pip install meson ninja
   ```
4. Install vcpkg and GLFW:
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg install glfw3:x64-windows
   ```
5. Set environment variable or use `--pkg-config-path` when configuring

**Option 3: Chocolatey**

```cmd
choco install python meson ninja
# Then install GLFW manually or via vcpkg
```

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential meson ninja-build libglfw3-dev libgl1-mesa-dev libx11-dev
```

### Fedora/RHEL

```bash
sudo dnf install gcc-c++ meson ninja-build glfw-devel mesa-libGL-devel libX11-devel
```

### Arch Linux

```bash
sudo pacman -S base-devel meson ninja glfw-x11 mesa libx11
```

### macOS

```bash
brew install meson ninja glfw
```

## Project Structure

```
.
├── src/                    # Application source files (.cpp)
├── include/                # Header files
├── external/
│   ├── glad/              # GLAD OpenGL loader
│   │   ├── include/
│   │   └── src/
│   └── stb_image/         # STB image loading library
├── meson.build            # Build configuration
└── README.md
```

## Building

### Setup

**Linux/macOS:**
```bash
meson setup build
```

**Windows (MSYS2):**
```bash
meson setup build
```

**Windows (Visual Studio):**
```cmd
meson setup build --backend=vs
# Or for Ninja backend:
meson setup build
```

**Windows (with vcpkg):**
```cmd
meson setup build --pkg-config-path=C:\path\to\vcpkg\installed\x64-windows\lib\pkgconfig
```

### Compile

**Linux/macOS/Windows (MSYS2):**
```bash
meson compile -C build
```

Or using Ninja directly:
```bash
ninja -C build
```

**Windows (Visual Studio):**
Open `build/opengl-app.sln` in Visual Studio and build, or use:
```cmd
meson compile -C build
```

### Run

**Linux/macOS:**
```bash
./build/app
```

**Windows:**
```cmd
.\build\app.exe
```

Or in MSYS2:
```bash
./build/app.exe
```

### Compile Commands

The build system generates `compile_commands.json` for IDE integration (clangd, IntelliSense, etc.).

### Clean Build

**Linux/macOS/Windows (MSYS2):**
```bash
rm -rf build
meson setup build
meson compile -C build
```

**Windows (PowerShell/CMD):**
```cmd
rmdir /s /q build
meson setup build
meson compile -C build
```

## Dependencies

- **GLFW3**: Cross-platform window and input handling
- **OpenGL**: Graphics API (opengl32 on Windows, GL on Linux, OpenGL framework on macOS)
- **GLAD**: OpenGL function loader
- **STB Image**: Image loading library
- **threads**: Threading library (POSIX threads on Unix, Windows threads on Windows)

### Platform-Specific Dependencies

- **Linux**: dl (dynamic linking), X11 (X Window System)
- **Windows**: opengl32 (OpenGL library)
- **macOS**: OpenGL framework

## Compiler Options

- C++ Standard: C++17
- Warning Level: 3 (high warnings enabled)
- GLAD built with reduced warnings (`-Wno-pedantic`)

## Platform Support

- **Linux**: Full support with X11
- **Windows**: Full support (MSVC, MinGW, MSYS2)
- **macOS**: Full support with Cocoa and OpenGL framework

## Acknowledgments

- [GLFW](https://www.glfw.org/) - Window and input library
- [GLAD](https://glad.dav1d.de/) - OpenGL loader generator
- [STB](https://github.com/nothings/stb) - Single-file public domain libraries