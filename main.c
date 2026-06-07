#include <stdio.h>
#include <stdlib.h>

#include "engine/board/bitboard.h"
#include "engine/movegen/gen_legal.h"
#include "engine/user/input.h"
#include "engine/util.h"

int main() {

    printf("Starting Honey..\n\n");

    board_state* b = malloc(sizeof(board_state));
    init_board(b);

    char* knight = "b1c3";

    struct move* knight_mov = get_squares(knight);
    
    PIECE piece_to = get_piece(b, knight_mov->from);
    printf("Piece Type: %s\n", piece_to_string(piece_to));

    uint64_t rook = 1;
    uint64_t full_board = 0;
    
    /* uint64_t rook_mask = rook_attack_mask(rook, full_board); */

    /* printf("\nRook attacks\n"); */
    /* print(rook_mask); */

    uint64_t* precomp_knight = precomp_knights();

    //Test: checks precomputed knight moves for pseudo legal moves in that square
    uint64_t square = precomp_knight[knight_mov->from];
    if(square & (1ULL << knight_mov->to)) {
        printf("Valid knight move");
    }

        
    /* for(int i = 0;i < 64;i++) { */
    /*     printf("Knight attacks for square %i\n", i); */
    /*     print(precomp_knight[i]); */
    /*  */
    /*     printf("\n"); */
    /* } */

        
    return 0;
}

