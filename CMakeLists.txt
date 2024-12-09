# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.20)

# Set compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Options
option(ZIV_ENABLE_OPTIMIZATIONS "Enable optimizations" ON)
option(ZIV_ENABLE_WARNINGS "Enable extra warnings" ON)
option(ZIV_ENABLE_TESTING "Enable testing" OFF)
option(ZIV_USE_CCACHE "Use ccache if available" ON)

# Use ccache if available
if(ZIV_USE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
        set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    else()
        message(WARNING "ccache not found, compilation will not be cached")
    endif()
endif()

project(Ziv VERSION 0.1.0 LANGUAGES CXX)

# Handle conda environment
if(DEFINED ENV{CONDA_PREFIX})
    set(CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX};${CMAKE_PREFIX_PATH}")
    link_directories("$ENV{CONDA_PREFIX}/lib")
    include_directories("$ENV{CONDA_PREFIX}/include")
    message(STATUS "Using Conda environment: $ENV{CONDA_PREFIX}")
endif()

# RPATH settings
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(DEFINED ENV{CONDA_PREFIX})
    file(TO_CMAKE_PATH "$ENV{CONDA_PREFIX}/lib" CONDA_LIB_DIR)
    list(APPEND CMAKE_INSTALL_RPATH "${CONDA_LIB_DIR}")
endif()

# C++ standard settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optimization flags
if(ZIV_ENABLE_OPTIMIZATIONS)
    set(OPTIMIZATION_FLAGS
        -O3
        -march=native
        -fno-rtti
        -ffunction-sections
    )
endif()

# Warning flags
if(ZIV_ENABLE_WARNINGS)
    set(WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wcast-qual
        -Wctor-dtor-privacy
        -Wsign-promo
        -Wdouble-promotion
    )
endif()

# Find required packages
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LLVM_INCLUDE_DIRS}
)

# Define source collection function
function(collect_sources RESULT PATH)
    file(GLOB_RECURSE _SOURCES
        ${PATH}/*.cpp
        ${PATH}/*.hpp
    )
    set(${RESULT} ${_SOURCES} PARENT_SCOPE)
endfunction()

# Collect sources
collect_sources(TOOLCHAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/toolchain")
collect_sources(ZIVC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zivc")

# Create the executable
add_executable(zivc
    ${TOOLCHAIN_SOURCES}
    ${ZIVC_SOURCES}
)

# Set compile options
target_compile_options(zivc PRIVATE
    ${WARNING_FLAGS}
    ${OPTIMIZATION_FLAGS}
)

# Set include directories
target_include_directories(zivc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link LLVM libraries
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    Option
)

target_link_libraries(zivc PRIVATE
    ${LLVM_LIBS}
)

# Testing
if(ZIV_ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "LLVM version: ${LLVM_VERSION}")
message(STATUS "Optimizations enabled: ${ZIV_ENABLE_OPTIMIZATIONS}")
message(STATUS "Warnings enabled: ${ZIV_ENABLE_WARNINGS}")
message(STATUS "Warnings as errors: ${ZIV_WARNINGS_AS_ERRORS}")
message(STATUS "Compiler flags: ${WARNING_FLAGS} ${OPTIMIZATION_FLAGS}")
if(DEFINED ENV{CONDA_PREFIX})
    message(STATUS "RPATH settings: ${CMAKE_INSTALL_RPATH}")
    message(STATUS "Conda lib dir: ${CONDA_LIB_DIR}")
endif()
