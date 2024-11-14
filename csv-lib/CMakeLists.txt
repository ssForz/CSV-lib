cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(test_task LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(example example.cpp csv_lib.cpp)


