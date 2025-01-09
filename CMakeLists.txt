# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#-------------------------------------------------------------------------------
# Basic Setup
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)

# Set compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(Ziv VERSION 0.1.0 LANGUAGES CXX)

#-------------------------------------------------------------------------------
# Build Options & Features
#-------------------------------------------------------------------------------
option(ZIV_ENABLE_OPTIMIZATIONS "Enable optimizations" ON)
option(ZIV_ENABLE_WARNINGS "Enable extra warnings" ON)
option(ZIV_ENABLE_TESTING "Enable testing" OFF)
option(ZIV_USE_CCACHE "Use ccache if available" ON)
option(ZIV_ENABLE_PCH "Enable Precompiled Headers" ON)
option(ZIV_ENABLE_LTO "Enable link-time optimization" ON)
option(ZIV_USE_THIN_LTO "Use ThinLTO instead of Full LTO" ON)
option(ZIV_ENABLE_HARDENING "Enable security hardening features" ON)

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

#-------------------------------------------------------------------------------
# Environment & System Setup
#-------------------------------------------------------------------------------
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
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------------------------------------------------------------------------
# Compilation Flags
#-------------------------------------------------------------------------------
# General compile flags
set(GENERAL_COMPILE_FLAGS
    -pipe   # Use pipes instead of temporary files
    -fdiagnostics-color=always  # Enable colored diagnostics
    $<$<CONFIG:Release>:-fno-math-errno> # Optimize math functions
)

# Optimization flags
if(ZIV_ENABLE_OPTIMIZATIONS)
    set(OPTIMIZATION_FLAGS
        -O3
        -march=native
        -fno-rtti
        -ffunction-sections
        -fdata-sections
        -fno-exceptions
        -ftree-vectorize
        -ffast-math
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

# Development flags
set(DEVELOPMENT_FLAGS
    -Werror=return-type
    -Wuninitialized
    -Wunused-parameter
    -Wconversion
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wfloat-equal
)

# Hardening flags
if(ZIV_ENABLE_HARDENING)
    set(HARDENING_FLAGS
        -fstack-protector-strong
        -Wformat
        -Wformat-security
        -fPIC
        $<$<CXX_COMPILER_ID:Clang>:-fsanitize-cfi-cross-dso>
    )
endif()

#-------------------------------------------------------------------------------
# Compiler Features Configuration
#-------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 2.6)
    if(CMAKE_HOST_WIN32 AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.7)
        list(APPEND GENERAL_COMPILE_FLAGS
            -fansi-escape-codes
            -fcolor-diagnostics
        )
    else()
        list(APPEND GENERAL_COMPILE_FLAGS
            -fcolor-diagnostics
        )
    endif()
endif()

#-------------------------------------------------------------------------------
# LTO Configuration
#-------------------------------------------------------------------------------

if(ZIV_ENABLE_LTO)
    # ThinLTO is available since Clang 3.9
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 3.9 AND ZIV_USE_THIN_LTO)
        add_compile_options(-flto=thin)
        add_link_options(
            -fuse-ld=lld
            -flto=thin
            LINKER:--thinlto-jobs=8
            LINKER:--gc-sections
            LINKER:--icf=all
        )
        message(STATUS "Using ThinLTO with lld")
    else()
        add_compile_options(-flto)
        add_link_options(
            -fuse-ld=lld
            -flto
            LINKER:--gc-sections
            LINKER:--icf=all
        )
        message(STATUS "Using Full LTO with lld")
    endif()
endif()

#-------------------------------------------------------------------------------
# Precompiled Headers Configuration
#-------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 11.0)
    if(ZIV_ENABLE_PCH)
        set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
    endif()
endif()

#-------------------------------------------------------------------------------
# Visibility Configuration
#-------------------------------------------------------------------------------
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
add_compile_options(-fvisibility=hidden)

#-------------------------------------------------------------------------------
# LLVM Setup
#-------------------------------------------------------------------------------
# LLVM configuration
set(LLVM_ENABLE_RTTI OFF)
set(LLVM_ENABLE_EH OFF)
set(LLVM_LINK_LLVM_DYLIB ON)

# Find required packages
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LLVM_INCLUDE_DIRS}
)

#-------------------------------------------------------------------------------
# Source & Target Configuration
#-------------------------------------------------------------------------------
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
    ${GENERAL_COMPILE_FLAGS}
    ${HARDENING_FLAGS}
    $<$<CONFIG:Debug>:${DEVELOPMENT_FLAGS}>
)

# Set target properties
set_target_properties(zivc PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION ${ZIV_ENABLE_LTO}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON
)

# Set include directories
target_include_directories(zivc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#-------------------------------------------------------------------------------
# Dependencies & Linking
#-------------------------------------------------------------------------------
# Link LLVM libraries
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    Option
)

target_link_libraries(zivc PRIVATE
    ${LLVM_LIBS}
)

#-------------------------------------------------------------------------------
# Testing Setup
#-------------------------------------------------------------------------------
if(ZIV_ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()

#-------------------------------------------------------------------------------
# Build Information
#-------------------------------------------------------------------------------
# Print build configuration
include(CMakePrintHelpers)
message(STATUS "=== Build Configuration ===")
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_ID)
cmake_print_variables(CMAKE_CXX_COMPILER_VERSION)
message(STATUS "=== Features ===")
cmake_print_variables(ZIV_ENABLE_LTO)
cmake_print_variables(ZIV_USE_THIN_LTO)
cmake_print_variables(ZIV_ENABLE_HARDENING)
cmake_print_variables(ZIV_ENABLE_OPTIMIZATIONS)
message(STATUS "=== Paths ===")
cmake_print_variables(CMAKE_INSTALL_PREFIX)
cmake_print_variables(CMAKE_INSTALL_RPATH)
if(DEFINED ENV{CONDA_PREFIX})
    message(STATUS "=== Conda Configuration ===")
    message(STATUS "RPATH settings: ${CMAKE_INSTALL_RPATH}")
    message(STATUS "Conda lib dir: ${CONDA_LIB_DIR}")
endif()
