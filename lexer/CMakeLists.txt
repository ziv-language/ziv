cmake_minimum_required(VERSION 3.10)
project(Lexer)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra)

find_package(GTest REQUIRED)

set(LEX_FILE ${CMAKE_SOURCE_DIR}/tokens.lex)

set(GENERATED_LEX_FILE ${CMAKE_BINARY_DIR}/lex.yy.cpp)

add_custom_command(
	OUTPUT ${GENERATED_LEX_FILE}
	COMMAND flex -o ${GENERATED_LEX_FILE} ${LEX_FILE}
	MAIN_DEPENDENCY ${LEX_FILE}
	COMMENT "Generating lexer with Flex"
)

add_library(lex_file OBJECT ${GENERATED_LEX_FILE})

add_library(tokens_file OBJECT tokens.cpp)

add_library(lexer_lib STATIC $<TARGET_OBJECTS:lex_file> $<TARGET_OBJECTS:tokens_file>)

add_executable(lexer_tests tests/lexer_test.cpp)
target_link_libraries(lexer_tests
	lexer_lib
	GTest::GTest
)

add_test(NAME RunTests COMMAND ${CMAKE_BINARY_DIR}/lexer_tests)
