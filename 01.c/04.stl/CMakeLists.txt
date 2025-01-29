cmake_minimum_required(VERSION 3.20)
project(CustomSTL C)

# Global compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Architecture specific flags for M1 Mac
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(00.core/01.memory_allocator)
add_subdirectory(02.vector)
add_subdirectory(03.linkedlist)