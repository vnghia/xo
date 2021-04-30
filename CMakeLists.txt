cmake_minimum_required(VERSION 3.17)
project(tic_tac_toe C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(tic_tac_toe main.c game.h game.c rendering.h rendering.c)

target_link_libraries(tic_tac_toe ${SDL2_LIBRARY} -mwindows)
