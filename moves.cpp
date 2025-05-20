#include "include/moves.hpp"

const uint64_t A_vertical = 0x0101010101010101ULL;
const uint64_t B_vertical = 0x0202020202020202ULL;
const uint64_t C_vertical = 0x0404040404040404ULL;
const uint64_t D_vertical = 0x0808080808080808ULL;
const uint64_t E_vertical = 0x1010101010101010ULL;
const uint64_t F_vertical = 0x2020202020202020ULL;
const uint64_t G_vertical = 0x4040404040404040ULL;
const uint64_t H_vertical = 0x8080808080808080ULL;

const uint64_t eight_horizontal =   0xFF00000000000000ULL;
const uint64_t seven_horizontal =   0xFF000000000000ULL;
const uint64_t six_horizontal =     0xFF0000000000ULL;
const uint64_t five_horizontal =    0xFF00000000ULL;
const uint64_t four_horizontal =    0xFF000000ULL;
const uint64_t three_horizontal =   0xFF0000ULL;
const uint64_t two_horizontal =     0xFF00ULL;
const uint64_t one_horizontal =     0xFFULL;

uint64_t Moves::king(uint64_t king, uint64_t board, uint64_t boardEnemy){
    if (king == 0) return 0;
    uint64_t move = 0ULL;
    move |= (king << 8); // up
    move |= (king >> 8); // down

    move |= (king << 1) & ~A_vertical; // right
    move |= (king >> 1) & ~H_vertical; // left

    move |= (king << 9) & ~A_vertical;
    move |= (king << 7) & ~H_vertical; 
    move |= (king >> 7) & ~A_vertical;
    move |= (king >> 9) & ~H_vertical;

    return move;
}

uint64_t Moves::queen(uint64_t queen, uint64_t board, uint64_t boardEnemy){
    if (queen == 0) return 0;
    return Moves::rook(queen, board, boardEnemy) | Moves::bishop(queen, board, boardEnemy);
}

uint64_t Moves::rook(uint64_t rook, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;

    int at_square = __builtin_ctzll(rook) + 1;
    uint64_t square = 1ULL << at_square;

    // up
    for (uint64_t potencial_move = square << 8; potencial_move != 0 && ((potencial_move >> 8) & eight_horizontal) == 0; potencial_move <<= 8) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down
    for (uint64_t potencial_move = square >> 8; potencial_move != 0&& ((potencial_move << 8) & one_horizontal) == 0; potencial_move >>= 8) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // right
    for (uint64_t potencial_move = square << 1; potencial_move != 0 && ((potencial_move >> 1) & H_vertical) == 0; potencial_move <<= 1) {
        if ((potencial_move & H_vertical) == H_vertical) break;
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // left
    for (uint64_t potencial_move = square >> 1; potencial_move != 0 && ((potencial_move << 1) & A_vertical) == 0; potencial_move >>= 1) {
        if ((potencial_move & A_vertical) == A_vertical) break;
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    return moves;
}

uint64_t Moves::allRooks(uint64_t rooks, uint64_t board, uint64_t boardEnemy) {
    if (rooks == 0) return 0;
    uint64_t move = 0ULL;
    while (rooks) {
        int sq = __builtin_ctzll(rooks);
        uint64_t single = 1ULL << sq;
        move |= rook(single, board, boardEnemy);
        rooks &= rooks - 1;
    }
    return move;
}

uint64_t Moves::bishop(uint64_t bishop, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;
    int at_square = __builtin_ctzll(bishop) + 1;
    uint64_t square = 1ULL << at_square;

    //up right
    for (uint64_t potencial_move = square << 9; potencial_move != 0 && ((potencial_move >> 9) & eight_horizontal) == 0  && ((potencial_move >> 9) & H_vertical) == 0; potencial_move <<= 9) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    //up left
    for (uint64_t potencial_move = square << 7; potencial_move != 0 && ((potencial_move >> 7) & eight_horizontal) == 0  && ((potencial_move >> 7) & A_vertical) == 0; potencial_move <<= 7) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down left
    for (uint64_t potencial_move = square >> 9; potencial_move != 0 && ((potencial_move << 9) & one_horizontal) == 0  && ((potencial_move << 9) & H_vertical) == 0; potencial_move >>= 9) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down right
    for (uint64_t potencial_move = square >> 7; potencial_move != 0 && ((potencial_move << 7) & eight_horizontal) == 0  && ((potencial_move << 7) & H_vertical) == 0; potencial_move >>= 7) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    return moves;
}

uint64_t Moves::allBishops(uint64_t bishops, uint64_t board, uint64_t boardEnemy) {
    if (bishops == 0) return 0;
    uint64_t move = 0ULL;
    while (bishops) {
        int sq = __builtin_ctzll(bishops);
        uint64_t single = 1ULL << sq;
        move |= bishop(single, board, boardEnemy);
        bishops &= bishops - 1;
    }
    return move;
}

uint64_t Moves::knight(uint64_t knight, uint64_t board, uint64_t boardEnemy) {
    if (knight == 0) return 0;
    uint64_t l1 = (knight >> 1) & (~A_vertical);
    uint64_t l2 = (knight >> 2) & (~H_vertical & ~G_vertical);
    uint64_t r1 = (knight << 1) & (~A_vertical);
    uint64_t r2 = (knight << 2) & (~A_vertical & ~B_vertical);

    return (l1 << 16) | (l2 << 8) | (r1 << 16) | (r2 << 8) |
           (l1 >> 16) | (l2 >> 8) | (r1 >> 16) | (r2 >> 8);
}

PawnMoves Moves::pawn(uint64_t pawn, uint64_t board, uint64_t boardEnemy, int color) {
    if (pawn == 0) return {0, 0, 0, 0};
     uint64_t forward = 0ULL, doubleForward = 0ULL, leftCapture = 0ULL, rightCapture = 0ULL;
     uint64_t empty = ~(board | boardEnemy);

        if (color == 1) {
        forward = (pawn << 8) & empty;
        doubleForward = ((pawn & two_horizontal) << 16) & empty & (empty << 8);
        leftCapture  = ((pawn & ~A_vertical) << 7) & boardEnemy;
        rightCapture = ((pawn & ~H_vertical) << 9) & boardEnemy;
    } else {
        forward = (pawn >> 8) & empty;
        doubleForward = ((pawn & seven_horizontal) >> 16) & empty & (empty >> 8);
        leftCapture  = ((pawn & ~H_vertical) >> 9) & board;
        rightCapture = ((pawn & ~A_vertical) >> 7) & board;
    }

    return { forward, doubleForward, leftCapture, rightCapture };
}

// board.whitePawn &= ~board.set_square(12);  // Remove from e2
// board.whitePawn |= board.set_square(28);   // Move to e4

// board.board &= ~board.set_square(12);
// board.board |= board.set_square(28);