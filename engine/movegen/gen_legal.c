#include "gen_legal.h"
#include "../board/bitboard.h"
#include <stdint.h>


#define DIRECTIONS 4
int directions[4] = {-8, 8, -1, 1};

const uint64_t NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
const uint64_t NOT_B_FILE  = 0xFDFDFDFDFDFDFDFDULL;
const uint64_t NOT_G_FILE  = 0xBFBFBFBFBFBFBFBFULL;
const uint64_t NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;

const uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
const uint64_t NOT_HG_FILE = 0x3F3F3F3F3F3F3F3FULL;

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

/*
    Precompute all pseudo-legal moves
    https://www.chessprogramming.org/Knight_Pattern
*/

uint64_t* precomp_knights() {

    uint64_t* precomp_knight = (uint64_t*) malloc(sizeof(uint64_t) * 64);
    
    //compute for all 64 squares
    for(size_t square = 0; square < 64;square++) {
        
        uint64_t attacks = 0;  
        uint64_t knight = 1ULL << square;

        //North moves
        if (((knight << 17) & NOT_A_FILE))  attacks |= (knight << 17); // NNE
        if (((knight << 15) & NOT_H_FILE))  attacks |= (knight << 15); // NNW
        if (((knight << 10) & NOT_AB_FILE)) attacks |= (knight << 10); // EEN
        if (((knight << 6)  & NOT_HG_FILE)) attacks |= (knight << 6);  // WWN

        //South moves
        if (((knight >> 6)  & NOT_AB_FILE)) attacks |= (knight >> 6);  // EES
        if (((knight >> 10) & NOT_HG_FILE)) attacks |= (knight >> 10); // WWS
        if (((knight >> 15) & NOT_A_FILE))  attacks |= (knight >> 15); // SSE
        if (((knight >> 17) & NOT_H_FILE))  attacks |= (knight >> 17); // SSW
          
        precomp_knight[square] = attacks;

    }
    
    return precomp_knight;

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

