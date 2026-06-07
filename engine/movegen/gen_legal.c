#include "gen_legal.h"
#include "../board/bitboard.h"
#include <stdint.h>


#define DIRECTIONS 4
int directions[4] = {-8, 8, -1, 1};

#define KNIGHT_MOVES 8
const int KNIGHT_OFFSETS[8] = {
    17,  
    15,  
    10,  
     6,  
    -6,  
   -10,  
   -15,  
   -17   
};

typedef void (*move_gen)(board_state*, uint64_t);
const move_gen generator_table[8] = {
    /* gen_pawn, */
    /* gen_knight, */
    /* gen_bishop, */
    gen_rook
    /* gen_queen, */
    /* gen_king, */
    /* gen_no_piece */
};

/*
   Returns index of piece
*/
uint64_t get_piece_square(uint64_t bitboard) {
    return __builtin_ctzll(bitboard);
}

void gen(board_state* board) {
    uint64_t full_board = get_full_board(board);
    for(int i = 0;i < 7;i++) {
        generator_table[i](board, full_board);
    }

}

uint64_t rook_attack_mask(uint64_t rook_board, uint64_t full_board) {
    uint64_t square = get_piece_square(rook_board);
    uint64_t attacks = 0;
    
    for(size_t i = 0; i < DIRECTIONS; i++) {
        int step = directions[i];
        uint64_t dir_sq = square + step;
        
        while(dir_sq < 64) {
            int prev_file = (dir_sq - step) % 8;
            int curr_file = dir_sq % 8;
            
            //check if dir_sq file is diff from prev
            if(abs(curr_file - prev_file) > 1) break;
            
            attacks |= (1ULL << dir_sq);
            
            //collision
            if((1ULL << dir_sq) & full_board) break;
            
            dir_sq += step;
        }
    }

    return attacks;
}

void gen_rook(board_state* board, uint64_t full_board) {
    
    for(int i = 0;i < N_ROOKS;i++) {
        
        uint64_t rook = board->pieces[board->turn][ROOK];
        uint64_t rook_attacks = rook_attack_mask(rook, full_board);

    }

}



void gen_knight(board_state *board) {

}

/* void gen_pawn(board_state *board) { */

/* } */

/* void gen_bishop(board_state *board) { */

/* } */

/* void gen_queen(board_state *board) { */

/* } */

/* void gen_king(board_state *board) { */

/* } */

/* void gen_no_piece(board_state board) { */
/*     printf("Failed"); */
/* } */

