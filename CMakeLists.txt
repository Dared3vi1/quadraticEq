cmake_minimum_required(VERSION 3.19)
project(QuadraticEq C)

set(CMAKE_C_STANDARD 99)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(QuadraticEq main.c)