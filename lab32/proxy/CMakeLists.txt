cmake_minimum_required(VERSION 3.23)
project(lab32)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab32 cpp/main.cpp hpp/constants.h hpp/proxy.h cpp/proxy.cpp)
