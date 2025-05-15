#include "moves.hpp"

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

uint64_t Moves::king(uint64_t king){
    return 0;
}

uint64_t Moves::queen(uint64_t queen){
    return 0;
}

uint64_t Moves::rook(uint64_t rook){
    return 0;
}

uint64_t Moves::bishop(uint64_t bishop){
    return 0;
}


uint64_t Moves::knight(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & (~A_vertical);
    uint64_t l2 = (knights >> 2) & (~H_vertical & ~G_vertical);
    uint64_t r1 = (knights << 1) & (~A_vertical);
    uint64_t r2 = (knights << 2) & (~A_vertical & ~B_vertical);

    return (l1 << 16) | (l2 << 8) | (r1 << 16) | (r2 << 8) |
           (l1 >> 16) | (l2 >> 8) | (r1 >> 16) | (r2 >> 8);
}

PawnMoves Moves::pawn(uint64_t pawn, uint64_t boardWhite, uint64_t boardBlack, int color) {
     uint64_t forward = 0ULL, doubleForward = 0ULL, leftCapture = 0ULL, rightCapture = 0ULL;
    uint64_t empty = ~(boardWhite | boardBlack);

        if (color == 1) {
        forward = (pawn << 8) & empty;
        doubleForward = ((pawn & two_horizontal) << 16) & empty & (empty << 8);
        leftCapture  = ((pawn & ~A_vertical) << 7) & boardBlack;
        rightCapture = ((pawn & ~H_vertical) << 9) & boardBlack;
    } else {
        forward = (pawn >> 8) & empty;
        doubleForward = ((pawn & seven_horizontal) >> 16) & empty & (empty >> 8);
        leftCapture  = ((pawn & ~H_vertical) >> 9) & boardWhite;
        rightCapture = ((pawn & ~A_vertical) >> 7) & boardWhite;
    }

    return { forward, doubleForward, leftCapture, rightCapture };
}

// board.whitePawn &= ~board.set_square(12);  // Remove from e2
// board.whitePawn |= board.set_square(28);   // Move to e4

// board.boardWhite &= ~board.set_square(12);
// board.boardWhite |= board.set_square(28);