cmake_minimum_required(VERSION 3.23)
project(lab32)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab32 cpp/proxy.cpp hpp/constants.h hpp/utils.h cpp/utils.cpp hpp/proxy_utils.h cpp/proxy_utils.cpp)
