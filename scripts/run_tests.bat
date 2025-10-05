@echo off
echo Running Computational Graphics Tests...

REM Create build directory for tests
if not exist build_tests mkdir build_tests
cd build_tests

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON

REM Build the tests
cmake --build . --config Release

REM Run the tests
echo.
echo Running tests...
bin\Release\ComputationalGraphicsTests.exe

echo.
echo Tests complete!
pause
