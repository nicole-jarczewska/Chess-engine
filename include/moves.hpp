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
    uint64_t king(uint64_t king, uint64_t board, uint64_t boardEnemy);
    uint64_t queen(uint64_t queen, uint64_t board, uint64_t boardEnemy);
    uint64_t rook(uint64_t rook, uint64_t board, uint64_t boardEnemy);
    uint64_t allRooks(uint64_t rooks, uint64_t board, uint64_t boardEnemy);
    uint64_t bishop(uint64_t bishop, uint64_t board, uint64_t boardEnemy);
    uint64_t allBishops(uint64_t rooks, uint64_t board, uint64_t boardEnemy);
    uint64_t knight(uint64_t knights, uint64_t board, uint64_t boardEnemy);
    PawnMoves pawn(uint64_t pawn, uint64_t board, uint64_t boardEnemy, int color);

private:
    
};
