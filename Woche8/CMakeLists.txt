cmake_minimum_required(VERSION 3.21)
project(BsuSp C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(BsuSp
        main.c
        mem_manager.h
        mem_test.c
        mem_test.h)
