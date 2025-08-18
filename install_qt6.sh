#!/bin/bash
#
# This script installs Qt6 development packages on Debian-based systems (like Ubuntu)
# and reports the location of the CMake configuration files.
#

set -e # Exit immediately if a command exits with a non-zero status.

echo "--- Updating package lists ---"
sudo apt-get update

echo "--- Installing Qt6 development packages ---"
# This will install the core Qt libraries, QML modules, and developer tools.
sudo apt-get install -y \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-tools-dev \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qml6-module-qtqml-workerscript \
    qml6-module-qtquick-templates \
    qml6-module-qtquick-window \
    qt6-tools-dev-tools

echo "--- Installation complete ---"
echo ""
echo "--- Locating Qt6 CMake configuration ---"

# Find the directory containing Qt6Config.cmake
QT6_CMAKE_PATH=$(dpkg -L qt6-base-dev | grep 'Qt6Config.cmake$' | xargs dirname)

if [ -z "$QT6_CMAKE_PATH" ]; then
    echo "Could not automatically find the Qt6 CMake directory."
    echo "Please find it manually. It's usually in a path like /usr/lib/x86_64-linux-gnu/cmake/Qt6"
else
    echo "Qt6 CMake configuration found at: $QT6_CMAKE_PATH"
    echo "You can now re-run the CMake command from your build directory."
    echo "Example: cmake .."
fi
