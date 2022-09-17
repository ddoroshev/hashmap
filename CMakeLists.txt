cmake_minimum_required(VERSION 3.23)
project(hashmap_cpp)
set(CMAKE_CXX_STANDARD 17)

include_directories(./)
add_executable(hashmap_cpp main.cpp dump.hpp hashmap/hashmap.hpp)
add_executable(test tests/test.cpp)
add_executable(playground playground.cpp)
