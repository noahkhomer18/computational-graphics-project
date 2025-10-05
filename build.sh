#!/bin/bash

echo "Building Computational Graphics Project..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make -j$(nproc)

echo "Build complete! Executable is in build/bin/"
