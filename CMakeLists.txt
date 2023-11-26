cmake_minimum_required(VERSION 3.22)
project(matrix)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
)

add_executable(matrix ${SRCS})
target_include_directories(matrix PRIVATE ../src)
