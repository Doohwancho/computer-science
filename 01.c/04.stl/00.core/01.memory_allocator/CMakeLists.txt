cmake_minimum_required(VERSION 3.10)
project(CustomAllocator C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compile options
add_compile_options(-Wall -Wextra -Wpedantic)

# Create library target
add_library(allocator
    src/allocator.c
)

# Set include directories for the library
target_include_directories(allocator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create test executable
add_executable(allocator_test
    tests/test_allocator.c
)

# Link test executable with the allocator library
target_link_libraries(allocator_test
    PRIVATE
        allocator
)

# Enable testing
enable_testing()
add_test(NAME AllocatorTest COMMAND allocator_test)