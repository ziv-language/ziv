# Create a library from the source files
add_library(source_lib
    source_buffer.cpp
    source_buffer.hpp
)

# Optionally add an executable for testing
add_executable(source_buffer_test source_buffer_test.cpp)

# Link the source library to the test executable
target_link_libraries(source_buffer_test
    PRIVATE
    source_lib
    MLIR
    LLVM
    LLVMCore
    LLVMSupport
)

# Set properties to specify output directories
set_target_properties(source_buffer_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
)

