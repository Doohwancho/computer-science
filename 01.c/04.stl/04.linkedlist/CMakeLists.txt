cmake_minimum_required(VERSION 3.20)
project(LinkedList C)

# 메모리 할당자 include 경로 추가
set(MEMORY_ALLOCATOR_DIR ${CMAKE_SOURCE_DIR}/../00.core/01.memory_allocator)

# LinkedList 라이브러리 생성
add_library(linkedlist STATIC
    src/linkedlist.c
)

# include 디렉토리 설정
target_include_directories(linkedlist
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${MEMORY_ALLOCATOR_DIR}/include  # 메모리 할당자 헤더 경로
)

# 메모리 할당자 라이브러리 링크
find_library(MEMORY_ALLOCATOR_LIB
    NAMES memory_allocator
    PATHS ${MEMORY_ALLOCATOR_DIR}/lib
)

target_link_libraries(linkedlist
    PRIVATE ${MEMORY_ALLOCATOR_LIB}
)

# 테스트 실행 파일 생성
add_executable(test_linkedlist
    tests/test_linkedlist.c
)

target_link_libraries(test_linkedlist
    PRIVATE linkedlist
    PRIVATE ${MEMORY_ALLOCATOR_LIB}
)

# 테스트 추가
add_test(NAME test_linkedlist COMMAND test_linkedlist)