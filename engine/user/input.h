#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../board/bitboard.h"


/*
   Create a mask using @param square to find which piece-type occupies that square
*/
PIECE get_piece(board_state* board, int square);

/*
    Returns the corresponding file number (0-based for bits) for the file name a = 0, h = 7
*/
int get_file_index(char file);


/*
    Returns a struct containing FROM and TO squares 0-based for bits
*/
move* get_squares(char* input);

