# Game Engine

## Compilation Instructions

1. **Prerequisites**:
    - Windows OS required.
    - MinGW64 compiler installed and in system PATH.
    - CMake installed (version 3.16 or higher).
    - SDL3 and GLAD libraries are included in the project.

2. **Building the Project**:
    - Open Command Prompt and navigate to the project directory:
      ```bash
      cd e:\c++\game_engine
      ```
    - Run the build script:
      ```bash
      clean_build.bat
      ```
    - This will create a `build` directory and compile the project with CMake and MinGW64.

3. **Cleaning the Build**:
    - To remove build artifacts and start fresh:
      ```bash
      clean_build.bat
      ```

4. **Running the Application**:
    - After building, the compiled executable will be available in the `build` directory.