cmake_minimum_required(VERSION 3.10)
project(honey C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(
    honey
    main.c 
    engine/board/bitboard.c
    engine/board/precomp.c
    engine/movegen/gen_legal.c
    engine/user/input.c
    engine/util.c

)


target_compile_options(honey PRIVATE -Wall -Wextra)

