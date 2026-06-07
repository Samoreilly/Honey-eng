
#include "input.h"

#include "../board/bitboard.h"


PIECE get_piece(board_state* board, int square) {

    uint64_t mask = (1ULL << square);

    for(int i = 0;i < COLORS;i++) {

        if(mask & board->pieces[board->turn][i]) {
            return i;
        }
    }

    return NONE;
}

int get_file_index(char file) {

    switch(file) {

        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default: return -1;
    }
}

move* get_squares(char* input) {
    int len = strlen(input);
    if(len < 4 || len > 4) return NULL;

    move* mov = (move*) malloc(sizeof(move));

    int from_file_index = get_file_index(input[0]);
    int from_rank = (input[1] - '0') - 1;
    int from_square = from_rank * 8 + from_file_index;
    
    int to_file_index = get_file_index(input[2]);
    int to_rank = (input[3] - '0') - 1;
    int to_square = to_rank * 8 + to_file_index;
    
    mov->from = from_square;
    mov->to = to_square;

    return mov;
}



