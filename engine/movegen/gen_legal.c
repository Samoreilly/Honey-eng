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
    gen_pawn,
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

void gen_pawn(board_state* board) {

    const uint64_t TOP_RANK = 0xFF00000000000000;
    const uint64_t BOTTOM_RANK = 0xFF;

    uint64_t pawns = board->pieces[board->turn][PAWN];
    uint64_t enemy = get_opposite_board(board, !board->turn); 
    uint64_t full_board = get_full_board(board);

    const int step = board->turn == WHITE ? 8 : -8;
    const uint64_t rank = board->turn == WHITE ? TOP_RANK : BOTTOM_RANK;

    //TODO: add array of counters to track number of pieces per piece-type
    while(pawns) {

        uint64_t square = get_piece_square(pawns);
        uint64_t precomp_pawn = board->precomp_attacks[board->turn][PAWN][square];
      
        //pseudo-valid attacks for pawn
        uint64_t move = precomp_pawn & enemy;
        uint64_t pawn_bb = 1ULL << square;

        //check if bottom or top rank depending on color
        if(!(pawn_bb & rank)) {
            
            //checks if pawn push is occupied
            uint64_t push = board->turn == WHITE ? (pawn_bb << 8) : (pawn_bb >> 8);
            if(!(push & full_board)) {
                move |= push;
            }
        }

        add_legal(board, move, square);
    
        //clears bit to continue looping through board, get_piece_square() handles this
        pawns &= ~(1ULL << square);

    }

}

void add_legal(board_state* board, uint64_t piece, uint64_t source) {

    while(piece) {
        int square = get_piece_square(piece);

        board->legal_moves.legal[board->legal_moves.legal_idx++] = (struct move){source, square};
        
        piece &= ~(1ULL << square);
    }

}


void gen_knight(board_state* board) {

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

