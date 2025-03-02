cmake_minimum_required(VERSION 3.30)
project(runtime1)

set(CMAKE_CXX_STANDARD 23)
include_directories(include)


add_executable(runtime1 main.cpp
        func.h
        values.h)
