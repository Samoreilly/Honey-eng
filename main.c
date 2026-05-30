#include <stdio.h>
#include <stdlib.h>

#include "engine/board/bitboard.h"

int main() {

    printf("Starting Honey..\n\n");

    board_state* b = malloc(sizeof(board_state));
    
    init_board(b);

    return 0;
}
