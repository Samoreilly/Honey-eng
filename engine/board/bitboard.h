#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE} PIECE;
typedef enum { WHITE, BLACK } COLOR;

#define PIECES 6
#define COLORS 2

#define N_PAWNS   8
#define N_ROOKS   2
#define N_KNIGHTS 2
#define N_BISHOPS 2
#define N_QUEENS  1
#define N_KINGS   1

/*
 Bit masks for multiple purposes, validating pieces positions in movegen & pre-computation
*/

#define NOT_A_FILE  0xFEFEFEFEFEFEFEFEULL
#define NOT_B_FILE  0xFDFDFDFDFDFDFDFDULL
#define NOT_G_FILE  0xBFBFBFBFBFBFBFBFULL
#define NOT_H_FILE  0x7F7F7F7F7F7F7F7FULL

#define NOT_AB_FILE 0xFCFCFCFCFCFCFCFCULL
#define NOT_HG_FILE 0x3F3F3F3F3F3F3F3FULL

#define H_FILE   0x8080808080808080ULL
#define A_FILE   0x0101010101010101ULL
#define TOP_RANK 0xFF00000000000000ULL
#define BOT_RANK 0x00000000000000FFULL

/*
   Input formatted as e2e4, e1g1 for castling etc 
*/

typedef struct move {
    uint32_t from;
    uint32_t to;
} move;

typedef struct legal_list {
    move legal[256];
    size_t legal_idx;
} legal_list;

typedef struct {
    
    //accessed with  [COLOR][PIECE][SQUARE] 
    uint64_t*** precomp_attacks;
    
    struct legal_list legal_moves;
    
    uint64_t pieces[2][6];
    COLOR turn;
    bool can_castle[2]; //accessed through COLOR. 0 - white, 1 - black
    //en passant

} board_state;

void init_board(board_state* b);
void init_bishops(board_state* b);
void init_knights(board_state* b);
void init_rooks(board_state* b);
void init_pawns(board_state* b);
void init_kings(board_state* b);
void init_queens(board_state* b);

void view_board(board_state* b);
void print(uint64_t piece);

uint64_t get_full_board(board_state* board);
uint64_t get_opposite_board(board_state* board, COLOR clr);

