#pragma once

#include <cstdint>

// const uint64_t A_vertical = 0x0101010101010101ULL;
// const uint64_t B_vertical = 0x0202020202020202ULL;
// const uint64_t G_vertical = 0x4040404040404040ULL;
// const uint64_t H_vertical = 0x8080808080808080ULL;

inline constexpr uint64_t A_vertical = 0x8080808080808080ULL;
inline constexpr uint64_t B_vertical = 0x4040404040404040ULL;
inline constexpr uint64_t G_vertical = 0x0202020202020202ULL;
inline constexpr uint64_t H_vertical = 0x0101010101010101ULL;

inline constexpr uint64_t eight_horizontal =   0xFF00000000000000ULL;
inline constexpr uint64_t seven_horizontal =   0xFF000000000000ULL;
inline constexpr uint64_t two_horizontal =     0xFF00ULL;
inline constexpr uint64_t one_horizontal =     0xFFULL;

class Moves {
public:
    uint64_t kingMoves(uint64_t king, uint64_t board, uint64_t boardEnemy);
    uint64_t queenMoves(uint64_t queen, uint64_t board, uint64_t boardEnemy);
    uint64_t rookMoves(uint64_t rook, uint64_t board, uint64_t boardEnemy);
    uint64_t allRooksMoves(uint64_t rooks, uint64_t board, uint64_t boardEnemy);
    uint64_t bishopMoves(uint64_t bishop, uint64_t board, uint64_t boardEnemy);
    uint64_t allBishopsMoves(uint64_t rooks, uint64_t board, uint64_t boardEnemy);
    uint64_t knightMoves(uint64_t knights, uint64_t board, uint64_t boardEnemy);
    uint64_t pawnMoves(uint64_t pawn, uint64_t board, uint64_t boardEnemy, int color);

    bool isCheck(uint64_t king, uint64_t moveEnemy);
    bool isCheckmate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy, 
        int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy);
    bool isStalemate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy);
};
