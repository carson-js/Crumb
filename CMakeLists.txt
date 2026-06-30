cmake_minimum_required(VERSION 4.2)
project(Crumb C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(Crumb main.c
        cpu.c)

target_link_libraries(Crumb SDL2::SDL2)
