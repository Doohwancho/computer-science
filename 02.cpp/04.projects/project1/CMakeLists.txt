# Define the executable for project1
add_executable(project1 main.cpp)

# Link the custom STL library
target_link_libraries(project1 PRIVATE my_stl)

# Include STL headers
target_include_directories(project1 PRIVATE ${CMAKE_SOURCE_DIR}/stl/include)
