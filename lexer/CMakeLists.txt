cmake_minimum_required(VERSION 3.10)
project(lexer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(SRC_DIR src)
set(INCLUDE_DIR include)

file(GLOB SOURCES "${SRC_DIR}/*.cpp")

include_directories(${INCLUDE_DIR})

add_library(lexer STATIC ${SOURCES})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
