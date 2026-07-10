cmake_minimum_required(VERSION 3.16)
project(sched_algos CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2 /W4)
else()
    add_compile_options(-O2 -Wall -Wextra)
endif()

add_executable(sched main.cpp)
target_include_directories(sched PRIVATE ${CMAKE_SOURCE_DIR})
