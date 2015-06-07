cmake_minimum_required(VERSION 3.2)
project(OS4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(OS4 ${SOURCE_FILES})
