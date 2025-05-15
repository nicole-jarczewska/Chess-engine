#pragma once

#include <cstdint>

struct PawnMoves {
    uint64_t forward;
    uint64_t doubleForward;
    uint64_t leftCapture;
    uint64_t rightCapture;
};

class Moves {
public:
    uint64_t king(uint64_t king);
    uint64_t queen(uint64_t queen);
    uint64_t rook(uint64_t rook);
    uint64_t bishop(uint64_t bishop);
    uint64_t knight(uint64_t knights);
    PawnMoves pawn(uint64_t pawn, uint64_t boardWhite, uint64_t boardBlack, int color);

private:
    
};
