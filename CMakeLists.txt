cmake_minimum_required(VERSION 4.2)
project(Crumb C)

set(CMAKE_C_STANDARD 11)

add_executable(Crumb main.c
        cpu.c)
