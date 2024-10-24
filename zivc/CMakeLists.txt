# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

file(GLOB_RECURSE COMMAND_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/command/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/command/*.hpp"
)

file(GLOB_RECURSE MAIN_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# Add them to parent scope
set(ZIVC_SOURCES
    ${COMMAND_SOURCES}
    ${MAIN_SOURCES}
    PARENT_SCOPE
)

# Add include directories
target_include_directories(zivc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
