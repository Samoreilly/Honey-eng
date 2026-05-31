
#include "board/bitboard.h"



const char* piece_to_string(PIECE piece) {
    switch (piece) {
        case PAWN:   return "PAWN";
        case KNIGHT: return "KNIGHT";
        case BISHOP: return "BISHOP";
        case ROOK:   return "ROOK";
        case QUEEN:  return "QUEEN";
        case KING:   return "KING";
        default:     return "NONE";
    }
}
