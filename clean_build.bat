rmdir /Q /S build >nul 2>&1
mkdir build >nul 2>&1
cd build >nul 2>&1
cmake .. -G "MinGW Makefiles"
cd .. >nul 2>&1
cmake --build build
del /S /Q build\resources >nul 2>&1
robocopy src\resources build\resources /E >nul 2>&1

copy external\SDL3\bin\SDL3.dll build\SDL3.dll

build\app.exe