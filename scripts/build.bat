@echo off
echo Building Computational Graphics Project...

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build the project
cmake --build . --config Release

echo Build complete! Executable is in build/bin/
pause