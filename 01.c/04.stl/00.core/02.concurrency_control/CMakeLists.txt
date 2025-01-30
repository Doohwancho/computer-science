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
add_executable(test_sync
    tests/test_sync.c
)

target_link_libraries(test_sync
    PRIVATE sync
    PRIVATE ${CMAKE_THREAD_LIBS_INIT}
)

# Add test to CTest
enable_testing()
add_test(NAME test_sync COMMAND test_sync)
