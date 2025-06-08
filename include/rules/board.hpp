#pragma once

#include <cstdint>

class Board{
public:
    Board();
    void reset();

    uint64_t set_square(int squere);
    bool piece_present(uint64_t board, int squere);
    char piece_at_square(int square);

    void print_bitboard();
    void flip(bool isFlipped);

    uint64_t boardWhite;
    uint64_t boardBlack;

    uint64_t blackKing;
    uint64_t blackQueen;
    uint64_t blackRook;
    uint64_t blackBishop;
    uint64_t blackKnight;
    uint64_t blackPawn;

    uint64_t whiteKing;
    uint64_t whiteQueen;
    uint64_t whiteRook;
    uint64_t whiteBishop;
    uint64_t whiteKnight;
    uint64_t whitePawn;
};