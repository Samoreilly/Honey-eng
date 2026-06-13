#pragma once

#include "../user/input.h"
/*
   Precompute all 218 legal moves for current position into a set
   Later used to check if users move is in that set
*/

#define likely(x) __builtin_expect(!!(x), 1)

/*
   Struct will contain all legal moves, 218 is the theoretical limit for a single position
*/
void gen(board_state* board);

void gen_knight(board_state*   board);
void gen_bishop(board_state*   board);
void gen_king(board_state*     board);
void gen_queen(board_state*    board);
void gen_rook(board_state*     board);
void gen_pawn(board_state*     board);
void gen_no_piece(board_state* board);

void add_legal(board_state* board, uint64_t piece, uint64_t source);

/*
    Gives us all possible attacks for Rook's current position
*/
uint64_t rook_attack_mask(uint64_t board, uint64_t full_board);
uint64_t get_piece_square(uint64_t bitboard);

