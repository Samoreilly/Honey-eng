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


void gen_knight(board_state* board) {
    
    uint64_t knight     = board->pieces[board->turn][KNIGHT]; 
    uint64_t opposition = get_opposite_board(board, !board->turn);
    uint64_t full_board = get_full_board(board);

    while(knight) {
    
        int square = get_piece_square(knight);
        uint64_t precomp_knight = board->precomp_attacks[board->turn][KNIGHT][square];
    
        uint64_t unoccupied_moves = ~full_board;//unoccupied squares
        unoccupied_moves |= opposition;//add enemy squares you can capture

        uint64_t valid_moves = precomp_knight & unoccupied_moves;
        add_legal(board, valid_moves, square);

        knight &= ~(1ULL << square);

    }
}

//TODO: unsure where king checking will be in pipeline, this is a reminder to do that
void gen_king(board_state* board) {

    uint64_t king       = board->pieces[board->turn][KING];
    uint64_t opposition = get_opposite_board(board, !board->turn);
    uint64_t full_board = get_full_board(board);
    
    uint64_t curr_color_board = full_board & ~opposition;

    while(king) {
        int square = get_piece_square(king);
        uint64_t precomp_king = board->precomp_attacks[board->turn][KING][square];
        
        //excludes curr color board
        uint64_t moves = precomp_king & ~(curr_color_board);
        add_legal(board, moves, square);

        king &= ~(1ULL << square);
    }

}

void gen_bishop(board_state* board) {
    const uint64_t H_FILE   = 0x8080808080808080ULL;
    const uint64_t A_FILE   = 0x0101010101010101ULL;
    const uint64_t TOP_RANK = 0xFF00000000000000ULL;
    const uint64_t BOT_RANK = 0x00000000000000FFULL;

    const int dirs[4] = {9, -7, -9, 7};
    const uint64_t dir_rules[4] = {
        TOP_RANK | H_FILE,
        BOT_RANK | H_FILE,
        BOT_RANK | A_FILE,
        TOP_RANK | A_FILE,
    };

    uint64_t bishop           = board->pieces[board->turn][BISHOP];
    uint64_t opposition       = get_opposite_board(board, !board->turn);
    uint64_t full_board       = get_full_board(board);
    uint64_t curr_color_board = full_board & ~opposition;
    
    while (bishop) {
        int square = get_piece_square(bishop);

        uint64_t attacks = 0;

        for (int dir = 0; dir < 4;dir++) {
            uint64_t n_square = 1ULL << square;
            uint64_t rule = dir_rules[dir];

            if (n_square & rule) continue;

            for (int step = 0; step < 7; step++) {

                if (dirs[dir] > 0) {
                    n_square <<= dirs[dir];
                }else {
                    n_square >>= (-dirs[dir]);
                }

                //stop at same color piece
                if (n_square & curr_color_board) break;
                
                //include curr square as taking were enemy piece
                attacks |= n_square;

                if (n_square & opposition) break;

                if (n_square & rule) break;

            }
        }
        add_legal(board, attacks, square);

        bishop &= ~(1ULL << square);
    }

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

void add_legal(board_state* board, uint64_t piece, uint64_t source) {

    while(piece) {
   
        uint32_t square = get_piece_square(piece);
        board->legal_moves.legal[board->legal_moves.legal_idx++] = (struct move){source, square};
        piece &= ~(1ULL << square);
    }

}

