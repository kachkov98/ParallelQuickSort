cmake_minimum_required(VERSION 2.6)
project(threadpool)

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -pthread -O2")

add_library(threadpool STATIC threadpool.cpp)

add_executable(qsort qsort.cpp)
target_link_libraries(qsort threadpool)
