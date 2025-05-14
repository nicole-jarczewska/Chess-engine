#pragma once

#include <cstdint>

class Moves {
public:
    uint64_t king(int x, int y, int color);
    uint64_t queen();
    uint64_t rook();
    uint64_t bishop();
    uint64_t knight(uint64_t knights);
    uint64_t pawn(uint64_t pawn, int color);

private:
    
};