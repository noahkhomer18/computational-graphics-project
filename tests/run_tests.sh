#!/bin/bash

echo "Running Computational Graphics Tests..."

# Create build directory for tests
mkdir -p build_tests
cd build_tests

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the tests
make -j$(nproc)

# Run the tests
echo ""
echo "Running tests..."
./bin/ComputationalGraphicsTests

echo ""
echo "Tests complete!"
