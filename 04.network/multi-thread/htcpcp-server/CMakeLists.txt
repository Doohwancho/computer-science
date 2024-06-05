cmake_minimum_required(VERSION 3.0.0)
project(server)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

find_package(unofficial-http-parser CONFIG REQUIRED)
find_package(pthreads REQUIRED)
find_library(WS2_32_LIBRARY ws2_32 REQUIRED)

add_executable(${PROJECT_NAME}
    src/kvp_list.c
    src/main.c
    src/queue.c
    src/request.c
    src/response.c
    src/string_utils.c
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        unofficial::http_parser::http_parser
        ${PTHREADS_LIBRARY}
        ws2_32
)
