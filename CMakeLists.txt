cmake_minimum_required(VERSION 3.10)
project(honey C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(
    honey
    main.c 
    engine/board/bitboard.c
)

