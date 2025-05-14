#include "moves.hpp"

const uint64_t A_vertical = 0x8080808080808080ULL;
const uint64_t B_vertical = 0x4040404040404040ULL;
const uint64_t C_vertical = 0x2020202020202020ULL;
const uint64_t D_vertical = 0x1010101010101010ULL;
const uint64_t E_vertical = 0x0808080808080808ULL;
const uint64_t F_vertical = 0x0404040404040404ULL;
const uint64_t G_vertical = 0x0202020202020202ULL;
const uint64_t H_vertical = 0x0101010101010101ULL;

const uint64_t eight_horizontal = 0xFF00000000000000ULL;
const uint64_t seven_horizontal = 0x7F00000000000000ULL;
const uint64_t six_horizontal = 0x3F00000000000000ULL;
const uint64_t five_horizontal = 0x1F00000000000000ULL;
const uint64_t four_horizontal = 0x0F00000000000000ULL;
const uint64_t three_horizontal = 0x07F0000000000000ULL;
const uint64_t two_horizontal = 0x03F0000000000000ULL;
const uint64_t one_horizontal = 0x01F0000000000000ULL;

uint64_t Moves::pawn(uint64_t pawn, int color) {
    if (color == 1) { // White
        return (pawn << 8) & (0xFFFFFFFFFFFFFFFFULL ^ eight_horizontal);
    } else { // Black
        return (pawn >> 8) & (0xFFFFFFFFFFFFFFFFULL ^ one_horizontal);
    }
}

uint64_t Moves::knight(uint64_t knights) {
    uint64_t l1 = (knights >> 1) & (0xFFFFFFFFFFFFFFFFULL ^ A_vertical);
    uint64_t l2 = (knights >> 2) & (0xFFFFFFFFFFFFFFFFULL ^ H_vertical & 0xFFFFFFFFFFFFFFFFULL ^ G_vertical);
    uint64_t r1 = (knights << 1) & (0xFFFFFFFFFFFFFFFFULL ^ A_vertical);
    uint64_t r2 = (knights << 2) & (0xFFFFFFFFFFFFFFFFULL ^ A_vertical & 0xFFFFFFFFFFFFFFFFULL ^ B_vertical);

    return (l1 << 16) | (l2 << 8) | (r1 << 16) | (r2 << 8) |
           (l1 >> 16) | (l2 >> 8) | (r1 >> 16) | (r2 >> 8);
}