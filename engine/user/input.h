#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../board/bitboard.h"

/*
   Input formatted as e2e4, e1g1 for castling etc 
*/

typedef struct move {
    int from;
    int to;
} move;


PIECE get_piece(board_state* board, int square);

int get_file_index(char file);
move* get_squares(char* input);

