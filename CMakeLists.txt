cmake_minimum_required(VERSION 3.22)
project(matrix)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
)

add_executable(matrix ${SRCS})
configure_file(test_matrix.py ${CMAKE_CURRENT_BINARY_DIR}/test_matrix.py COPYONLY)
target_include_directories(matrix PRIVATE ../src)
