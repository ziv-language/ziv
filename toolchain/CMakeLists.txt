# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Collect source files for each component
file(GLOB_RECURSE DRIVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/driver/*.hpp"
)

file(GLOB_RECURSE SOURCE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"
)

# Add them to parent scope
set(TOOLCHAIN_SOURCES
    ${DRIVER_SOURCES}
    ${SOURCE_SOURCES}
    PARENT_SCOPE
)

# Add include directories
target_include_directories(zivc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)
