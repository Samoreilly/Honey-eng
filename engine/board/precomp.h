#include <inttypes.h>

#include "bitboard.h"

uint64_t*** precomp();

uint64_t* precomp_knights();
uint64_t* precomp_white_pawns();
uint64_t* precomp_black_pawns();
uint64_t* precomp_bishops();
uint64_t* precomp_rooks();
uint64_t* precomp_king();
uint64_t* precomp_queens();
uint64_t* precomp_no_piece();

