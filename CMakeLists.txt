cmake_minimum_required(VERSION 3.22)
project(deepThonk3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically run MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt
find_package(Qt6 6.4.2 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

# Add source code
add_subdirectory(src)
