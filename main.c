#include <stdio.h>
#include <stdlib.h>

#include "engine/board/bitboard.h"
#include "engine/user/input.h"
#include "engine/util.h"

int main() {

    printf("Starting Honey..\n\n");

    board_state* b = malloc(sizeof(board_state));
    init_board(b);

    char* move = "e2e4";
    struct move* mov = get_squares(move);

    printf("From: %i\nTo: %i\n", mov->from, mov->to);

    PIECE piece = get_piece(b, mov->from);
    printf("Piece Type: %s\n", piece_to_string(piece));

    char* knight = "b1c3";

    struct move* knight_mov = get_squares(knight);
    
    PIECE piece_to = get_piece(b, knight_mov->from);
    printf("Piece Type: %s\n", piece_to_string(piece_to));

    return 0;
}

