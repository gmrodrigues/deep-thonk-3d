#!/bin/bash
#
# This script compiles and runs the deepThonk3d application.
#

set -e # Exit immediately if a command exits with a non-zero status.

# Create the build directory if it doesn't exist
mkdir -p build

# Navigate into the build directory
cd build

# Configure the project with CMake
# The CMAKE_PREFIX_PATH points to your Qt6 installation.
echo "--- Configuring project with CMake ---"
cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 ..

# Compile the project using all available processor cores
echo "--- Compiling project with make ---"
make -j$(nproc)

# Run the tests
echo "--- Running tests ---"
ctest --verbose

# Run the application
echo "--- Running application ---"
./src/deepThonk3d_app
