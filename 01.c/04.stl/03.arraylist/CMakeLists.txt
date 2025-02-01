cmake_minimum_required(VERSION 3.20)
project(ArrayList C)

# Memory allocator and synchronization primitives directories
set(MEMORY_ALLOCATOR_DIR ${CMAKE_SOURCE_DIR}/../00.core/01.memory_allocator)
set(SYNC_DIR ${CMAKE_SOURCE_DIR}/../00.core/02.concurrency_control)

# Add synchronization primitives library
add_library(sync STATIC
    ${SYNC_DIR}/src/my_mutex.c
    ${SYNC_DIR}/src/my_condvar.c
    ${SYNC_DIR}/src/my_rwlock.c
)

target_include_directories(sync
    PUBLIC ${SYNC_DIR}/include
)

# Add ArrayList library
add_library(arraylist STATIC
    src/my_arraylist.c
)

target_include_directories(arraylist
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${MEMORY_ALLOCATOR_DIR}/include
        ${SYNC_DIR}/include
)

# Find memory allocator library
find_library(MEMORY_ALLOCATOR_LIB
    NAMES memory_allocator
    PATHS ${MEMORY_ALLOCATOR_DIR}/lib
)

target_link_libraries(arraylist
    PRIVATE ${MEMORY_ALLOCATOR_LIB}
    PRIVATE sync
)

# Tests
add_executable(test_arraylist
    tests/test_arraylist.c
)

target_link_libraries(test_arraylist
    PRIVATE arraylist
    PRIVATE ${MEMORY_ALLOCATOR_LIB}
    PRIVATE sync
    PRIVATE pthread
)

# Add test
add_test(NAME test_arraylist COMMAND test_arraylist)
