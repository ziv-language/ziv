# Part of the Ziv Programming Language, under the Apache License v2.0 with LLVM
# See /LICENSE for license details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Collect test source files
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/toolchain/*.cpp"
)

# Collect the source files needed for testing
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/toolchain/source/*.cpp"
)

# Create the test executable with both test and implementation files
add_executable(ziv_tests 
    ${TEST_SOURCES}
    ${SOURCE_FILES}
)


# Link against Google Test and your project's libraries
target_link_libraries(ziv_tests PRIVATE
    GTest::gtest_main
    ${LLVM_LIBS}
    LLVMCore
    LLVMSupport
    LLVMOption
)

# Add include directories
target_include_directories(ziv_tests PRIVATE
    ${CMAKE_SOURCE_DIR}  # Root project directory
    ${CMAKE_SOURCE_DIR}/toolchain
    ${CMAKE_SOURCE_DIR}/toolchain/source
    ${CMAKE_SOURCE_DIR}/toolchain/lex
    ${CMAKE_SOURCE_DIR}/toolchain/ast
    ${CMAKE_SOURCE_DIR}/toolchain/parser
    ${CMAKE_SOURCE_DIR}/toolchain/checker
)

# Enable test discovery
include(GoogleTest)
gtest_discover_tests(ziv_tests)
