
#include <stdio.h>

#include "bitboard.h"

void init_board(board_state* b) {

    b->can_castle[0] = true;
    b->can_castle[1] = true;
    b->turn = WHITE;

    for(size_t i = 0;i < 2;i++) {
        for(size_t j = 0;j < 6;j++) {
            b->pieces[i][j] = 0;
        } 
    }

    init_bishops(b);
    init_knights(b);
    init_rooks(b);
    init_queens(b);
    init_kings(b);
    init_pawns(b);  

    print(b->pieces[WHITE][QUEEN]);

    view_board(b);

}

void init_bishops(board_state* b) {

    b->pieces[WHITE][BISHOP] |= (1ULL << 2);
    b->pieces[WHITE][BISHOP] |= (1ULL << 5);

    b->pieces[BLACK][BISHOP] |= (1ULL << 61);
    b->pieces[BLACK][BISHOP] |= (1ULL << 58);

}

void init_knights(board_state* b) {

    b->pieces[WHITE][KNIGHT] |= (1ULL << 1);
    b->pieces[WHITE][KNIGHT] |= (1ULL << 6);

    b->pieces[BLACK][KNIGHT] |= (1ULL << 57);
    b->pieces[BLACK][KNIGHT] |= (1ULL << 62);

}

void init_rooks(board_state* b) {

    b->pieces[WHITE][ROOK] |= (1ULL << 0);
    b->pieces[WHITE][ROOK] |= (1ULL << 7);

    b->pieces[BLACK][ROOK] |= (1ULL << 56);
    b->pieces[BLACK][ROOK] |= (1ULL << 63);

}

void init_queens(board_state* b) {

    b->pieces[WHITE][QUEEN] |= (1ULL << 3);
    b->pieces[BLACK][QUEEN] |= (1ULL << 59);
}

void init_kings(board_state* b) {

    b->pieces[WHITE][KING] |= (1ULL << 4);
    b->pieces[BLACK][KING] |= (1ULL << 60);

}

void init_pawns(board_state* b) {
    
    uint64_t white_pawn_mask = 0xFF00;
    uint64_t black_pawn_mask = 0xFF000000000000;

    b->pieces[WHITE][PAWN] |= white_pawn_mask; 
    b->pieces[BLACK][PAWN] |= black_pawn_mask;
}

void view_board(board_state* b) {
    uint64_t board = 1ULL;

    for(int i = 0;i < COLORS;i++) {
        for(int j = 0;j < PIECES;j++) {
            board |= b->pieces[i][j];
        }
    }

    printf("\nFull Board\n\n");
    print(board);
}

void print(uint64_t piece) {

    for(int rank = 7; rank >= 0; rank--) {
        for(int file = 0; file < 8; file++) {
        
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            
            printf("%c", (piece & mask) ? '1' : '0');
        }
        printf("\n");
    }
}




