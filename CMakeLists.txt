cmake_minimum_required(VERSION 2.6)
project(threadpool)

add_executable(threadpool main.cpp)

install(TARGETS threadpool RUNTIME DESTINATION bin)
