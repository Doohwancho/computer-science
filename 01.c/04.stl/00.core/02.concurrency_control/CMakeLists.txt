# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(Synchronization C)

# Compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Architecture specific flags for M1 Mac
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# Source files
set(SYNC_SOURCES
    src/my_mutex.c
    src/my_condvar.c
    src/my_rwlock.c
)

# Create static library
add_library(sync STATIC ${SYNC_SOURCES})

target_include_directories(sync
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Remove -Werror flag
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Link with pthread
find_package(Threads REQUIRED)
target_link_libraries(sync
    PRIVATE ${CMAKE_THREAD_LIBS_INIT}
)

# Tests
add_executable(test_sync tests/test_sync.c)
add_executable(test_mutex tests/test_mutex.c)
add_executable(test_condvar tests/test_condvar.c)
add_executable(test_rwlock tests/test_rwlock.c)


target_link_libraries(test_sync
    PRIVATE sync
    PRIVATE ${CMAKE_THREAD_LIBS_INIT}
)
target_link_libraries(test_mutex PRIVATE sync pthread)
target_link_libraries(test_condvar PRIVATE sync pthread)
target_link_libraries(test_rwlock PRIVATE sync pthread)


# Add test to CTest
enable_testing()
add_test(NAME test_sync COMMAND test_sync)
add_test(NAME test_mutex COMMAND test_mutex)
add_test(NAME test_condvar COMMAND test_condvar)
add_test(NAME test_rwlock COMMAND test_rwlock)
