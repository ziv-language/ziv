cmake_minimum_required(VERSION 3.15)
project(Ziv)

# Use Clang as the compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set LLVM and MLIR directories
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS} include)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Collect source files
file(GLOB_RECURSE COMPILER_SOURCES
    src/*.cpp
    include/*.hpp
)

# Add executable
add_executable(zivc ${COMPILER_SOURCES})

# Link LLVM and MLIR libraries
target_link_libraries(zivc
    PRIVATE
    MLIR
    LLVM
    LLVMCore
    LLVMSupport
    # Add other necessary LLVM/MLIR libraries
)

# Set output directory
set_target_properties(zivc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)