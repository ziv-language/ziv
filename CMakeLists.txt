# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(Ziv)

# Use Clang as the compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set C++17 as the standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM package
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})

# Define source collection functions
function(collect_sources RESULT PATH)
    file(GLOB_RECURSE _SOURCES
        ${PATH}/*.cpp
        ${PATH}/*.hpp
    )
    set(${RESULT} ${_SOURCES} PARENT_SCOPE)
endfunction()

# Collect all sources
collect_sources(TOOLCHAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
collect_sources(ZIVC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zivc")

# Create the executable
add_executable(zivc
    ${TOOLCHAIN_SOURCES}
    ${ZIVC_SOURCES}
)

# Group sources by module in IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${TOOLCHAIN_SOURCES}
    ${ZIVC_SOURCES}
)

# Add all include directories
target_include_directories(zivc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link LLVM libraries
target_link_libraries(zivc PRIVATE
    ${LLVM_LIBS}
    LLVMCore
    LLVMSupport
    LLVMOption
)

# Print summary of files found (useful for debugging)
message(STATUS "Found toolchain sources: ${TOOLCHAIN_SOURCES}")
message(STATUS "Found zivc sources: ${ZIVC_SOURCES}")


# Enable testing
enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(tests)
