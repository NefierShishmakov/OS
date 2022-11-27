cmake_minimum_required(VERSION 3.23)
project(lab31 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab31 h/hello.h c/main.c c/hello.c)
