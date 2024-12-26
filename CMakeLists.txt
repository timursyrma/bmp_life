cmake_minimum_required(VERSION 3.10)
project(bmp_life C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

add_subdirectory(src/)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} bmp_src)
