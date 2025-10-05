# Test configuration for Computational Graphics Project

cmake_minimum_required(VERSION 3.16)

# Find required packages
find_package(GTest REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)

# Test source files
set(TEST_SOURCES
    test_camera.cpp
    test_scene.cpp
    test_performance.cpp
)

# Create test executable
add_executable(ComputationalGraphicsTests ${TEST_SOURCES})

# Link libraries
target_link_libraries(ComputationalGraphicsTests
    GTest::GTest
    GTest::Main
    OpenGL::GL
    glfw
    GLEW::GLEW
)

# Add source files to test executable
target_sources(ComputationalGraphicsTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ViewManager.cpp
    ${CMAKE_SOURCE_DIR}/src/Object3D.cpp
    ${CMAKE_SOURCE_DIR}/src/Light.cpp
    ${CMAKE_SOURCE_DIR}/src/PerformanceMonitor.cpp
    ${CMAKE_SOURCE_DIR}/src/DebugRenderer.cpp
    ${CMAKE_SOURCE_DIR}/src/ShaderManager.cpp
)

# Set output directory
set_target_properties(ComputationalGraphicsTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable testing
enable_testing()

# Add test to CTest
add_test(NAME ComputationalGraphicsTests COMMAND ComputationalGraphicsTests)

# Set test properties
set_tests_properties(ComputationalGraphicsTests PROPERTIES
    TIMEOUT 60
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
