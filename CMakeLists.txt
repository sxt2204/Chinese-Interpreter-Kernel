cmake_minimum_required(VERSION 3.14)
project(ChineseCompiler VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include root directory for header resolution
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# MSVC UTF-8 encoding support for Chinese literals
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Main test executable
add_executable(test test.cpp)

# Output directory setting
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
