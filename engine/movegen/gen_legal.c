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
const uint64_t TOP_RANK = 0xFF00000000000000;
const uint64_t BOTTOM_RANK = 0xFF;

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
typedef uint64_t* (*precompute) (void);

const precompute precomp_table[PIECES + 1] = {
    precomp_pawns,
    precomp_knights,
    precomp_bishops,
    precomp_rooks,
    precomp_queens,
    precomp_king,
    precomp_no_piece
};

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



uint64_t** precomp() {

    uint64_t** precomp = calloc(PIECES, sizeof(uint64_t*));
    if(!precomp) {
        printf("Cannot precompute\n");
        return NULL;
    }

    for(size_t i = 0;i < PIECES;i++) {
        precomp[i] = precomp_table[i]();
    }

    return precomp;

}

/*
    Precompute all pseudo-legal moves
    https://www.chessprogramming.org/Knight_Pattern
*/

uint64_t* precomp_knights() {

    uint64_t* precomp_knight = (uint64_t*) malloc(sizeof(uint64_t) * 64);
    
    //compute for all 64 squares
    for(size_t square = 0;square < 64;square++) {
        
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

uint64_t* precomp_pawns() {

    uint64_t* precomp_pawns = (uint64_t*) calloc(64, sizeof(uint64_t));
    
    for(size_t square = 0; square < 64;square++) {
        
        uint64_t attacks = 0;
        uint64_t pawn = 1ULL << square;

        //stop computing when at top rank
        if(pawn & TOP_RANK) {
            break;
        }

        //7 is north east

        //check if A file, if not compute NE square
        if(pawn & NOT_A_FILE) attacks |= (pawn << 7);
        attacks |= (pawn << 8);
        if(pawn & NOT_H_FILE) attacks |= (pawn << 9);   

        precomp_pawns[square] = attacks;

    }

    return precomp_pawns;

}
 
uint64_t* precomp_rooks() {

}

uint64_t* precomp_king() {

    uint64_t* precomp = (uint64_t*) calloc(64, sizeof(uint64_t));

    for(size_t square = 0;square < 64;square++) {
        
        uint64_t attacks = 0;
        uint64_t pawn = 1ULL << square;

        //left side
        if(pawn & NOT_A_FILE) {
            if(!(pawn & TOP_RANK)) attacks |= (pawn << 7);//NW
            attacks |= (pawn >> 1);//W
            if(!(pawn & BOTTOM_RANK)) attacks |= (pawn >> 9);//SW
        }

        //top
        if(!(pawn & TOP_RANK)) attacks |= (pawn << 8);
        
        //right side
        if(pawn & NOT_H_FILE) {
            if(!(pawn & TOP_RANK)) attacks |= (pawn << 9);//NE
            attacks |= (pawn << 1);//E
            if(!(pawn & BOTTOM_RANK)) attacks |= (pawn >> 7);//SE
        }

        if(!(pawn & TOP_RANK)) attacks |= (pawn << 8);
        if(!(pawn & BOTTOM_RANK)) attacks |= (pawn >> 8);

        
        precomp[square] = attacks;
    }

    return precomp;
    
}

uint64_t* precomp_bishops() {

}

uint64_t* precomp_queens() {
}

uint64_t* precomp_no_piece() {
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

