#include <stdio.h>
#include <stdlib.h>

#include "precomp.h"
#include "bitboard.h"

uint64_t*** precomp() {

    uint64_t*** precomp = calloc(COLORS, sizeof(uint64_t**));
    if(!precomp) {
        printf("Cannot precompute\n");
        return NULL;
    }

    for(size_t c = 0; c < COLORS; c++) {
        precomp[c] = calloc(PIECES, sizeof(uint64_t*));

        if(!precomp[c]) {
            printf("Cannot precompute\n");
            return NULL;
        }
    }

    precomp[WHITE][PAWN] = precomp_white_pawns();
    precomp[BLACK][PAWN] = precomp_black_pawns();

    precomp[WHITE][KNIGHT] = precomp_knights();
    precomp[BLACK][KNIGHT] = precomp[WHITE][KNIGHT];

    precomp[WHITE][BISHOP] = precomp_bishops();
    precomp[BLACK][BISHOP] = precomp[WHITE][BISHOP];

    precomp[WHITE][ROOK] = precomp_rooks();
    precomp[BLACK][ROOK] = precomp[WHITE][ROOK];

    precomp[WHITE][QUEEN] = precomp_queens();
    precomp[BLACK][QUEEN] = precomp[WHITE][QUEEN];

    precomp[WHITE][KING] = precomp_king();
    precomp[BLACK][KING] = precomp[WHITE][KING];

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

uint64_t* precomp_black_pawns() {

    uint64_t* precomp_pawns = (uint64_t*) calloc(64, sizeof(uint64_t));

    for(size_t square = 0; square < 64;square++) {
        
        uint64_t attacks = 0;
        uint64_t pawn = 1ULL << square;

        //7 is north east

        //check if A file, if not compute SE square
        if(pawn & NOT_A_FILE) attacks |= (pawn >> 9);
        attacks |= (pawn >> 8);
        if(pawn & NOT_H_FILE) attacks |= (pawn >> 7);   

        precomp_pawns[square] = attacks;

    }

    return precomp_pawns;

}
uint64_t* precomp_white_pawns() {

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
 
uint64_t* precomp_king() {

    uint64_t* precomp = (uint64_t*) calloc(64, sizeof(uint64_t));

    for(size_t square = 0;square < 64;square++) {
        
        uint64_t attacks = 0;
        uint64_t pawn = 1ULL << square;

        //left side
        if(pawn & NOT_A_FILE) {
            if(!(pawn & TOP_RANK)) attacks |= (pawn << 7);//NW
            attacks |= (pawn >> 1);//W
            if(!(pawn & BOT_RANK)) attacks |= (pawn >> 9);//SW
        }
       
        //right side
        if(pawn & NOT_H_FILE) {
            if(!(pawn & TOP_RANK)) attacks |= (pawn << 9);//NE
            attacks |= (pawn << 1);//E
            if(!(pawn & BOT_RANK)) attacks |= (pawn >> 7);//SE
        }

        if(!(pawn & TOP_RANK)) attacks |= (pawn << 8);
        if(!(pawn & BOT_RANK)) attacks |= (pawn >> 8);

        
        precomp[square] = attacks;
    }

    return precomp;
    
}

uint64_t* precomp_bishops() { return NULL; }

uint64_t* precomp_queens() { return NULL; }

uint64_t* precomp_no_piece() { return NULL; }

uint64_t* precomp_rooks() { return NULL; }

