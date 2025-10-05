@echo off
echo Cleaning build directories...

REM Remove build directories
if exist build rmdir /s /q build
if exist build_tests rmdir /s /q build_tests

echo Clean complete!
pause
