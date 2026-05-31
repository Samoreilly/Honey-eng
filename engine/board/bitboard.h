#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef enum { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE} PIECE;
typedef enum { WHITE, BLACK } COLOR;

#define PIECES 6
#define COLORS 2

typedef struct {

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



