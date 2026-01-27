# Game Engine

## Compilation Instructions

1. **Prerequisites**:
    - Windows OS required.
    - MinGW64 compiler installed and in system PATH.
    - CMake installed (version 3.16 or higher).
    - SDL3 and GLAD libraries are included in the project.

2. **Building the Project**:
    - Clone the repo and cd into it:
      ```bash
      git clone https://github.com/skkydoesstuff/OpenGL.git
      cd OpenGL
      ```

    - Run the build script:
      ```bash
      clean_build.bat
      ```
    - This will create a `build` directory and compile the project with CMake and MinGW64.

3. **Building after initial build**:
    - To speed up compiliation after first build a bit:
      ```bash
      build.bat
      ```

4. **Running the Application**:
    - After building, the compiled executable will be available in the `build` directory.