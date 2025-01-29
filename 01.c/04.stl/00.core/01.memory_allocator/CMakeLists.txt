add_library(memory_allocator STATIC
    src/memory_allocator.c
)

target_include_directories(memory_allocator
    PUBLIC include
)

# Tests
add_executable(test_allocator
    tests/test_allocator.c
)

target_link_libraries(test_allocator
    PRIVATE memory_allocator
)

add_test(NAME test_allocator COMMAND test_allocator)