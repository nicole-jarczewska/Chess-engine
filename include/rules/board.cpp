﻿#include "board.hpp"

Board::Board(){
    boardWhite = 0x000000000000FFFFULL;
    boardBlack = 0xFFFF000000000000ULL;

    whiteKing = set_square(4);
    whiteQueen = set_square(3);
    whiteRook = set_square(0) | set_square(7);
    whiteBishop = set_square(2) | set_square(5);
    whiteKnight = set_square(1) | set_square(6);
    whitePawn = 0x000000000000FF00ULL;           

    blackKing = set_square(60);
    blackQueen = set_square(59);
    blackRook = set_square(56) | set_square(63);
    blackBishop = set_square(58) | set_square(61);
    blackKnight = set_square(57) | set_square(62);
    blackPawn = 0x00FF000000000000ULL;
}

void Board::reset() {
    boardWhite = 0x000000000000FFFFULL;
    boardBlack = 0xFFFF000000000000ULL;

    whiteKing = set_square(4);
    whiteQueen = set_square(3);
    whiteRook = set_square(0) | set_square(7);
    whiteBishop = set_square(2) | set_square(5);
    whiteKnight = set_square(1) | set_square(6);
    whitePawn = 0x000000000000FF00ULL;

    // whiteRook = set_square(44) | set_square(55);

    blackKing = set_square(60);
    blackQueen = set_square(59);
    blackRook = set_square(56) | set_square(63);
    blackBishop = set_square(58) | set_square(61);
    blackKnight = set_square(57) | set_square(62);
    blackPawn = 0x00FF000000000000ULL;
    // blackQueen = 0ULL;
    // blackRook = 0ULL;
    // blackBishop = 0ULL;
    // blackKnight = 0ULL;
    // blackPawn = 0ULL;
}

void Board::flip(bool isFlipped) {
    if(isFlipped){
        whiteKing = set_square(3);
        whiteQueen = set_square(4);
        blackKing = set_square(59);
        blackQueen = set_square(60);
    }
}

// Board::Board(){
//     whiteKing = 0x0ULL;
//     whiteQueen = 0x0ULL;
//     whiteRook = 0x0ULL;
//     whiteBishop = 0x0ULL;
//     whiteKnight = 0x0ULL;
//     whitePawn = set_square(17);           

//     blackKing = set_square(24);
//     blackQueen = 0x0ULL;
//     blackRook = 0x0ULL;
//     blackBishop = 0x0ULL;
//     blackKnight = 0x0ULL;
//     blackPawn = 0x0ULL;
// }

uint64_t Board::set_square(int squere) {
    return 1ULL << squere;
}

bool Board::piece_present(uint64_t board, int squere) {
    return (board & set_square(squere)) != 0;
}

char Board::piece_at_square(int square){
    if (blackPawn & (1ULL << square)) return 'p';
    if (blackKnight & (1ULL << square)) return 'n';
    if (blackBishop & (1ULL << square)) return 'b';
    if (blackRook & (1ULL << square)) return 'r';
    if (blackQueen & (1ULL << square)) return 'q';
    if (blackKing & (1ULL << square)) return 'k';

    if (whitePawn & (1ULL << square)) return 'P';
    if (whiteKnight & (1ULL << square)) return 'N';
    if (whiteBishop & (1ULL << square)) return 'B';
    if (whiteRook & (1ULL << square)) return 'R';
    if (whiteQueen & (1ULL << square)) return 'Q';
    if (whiteKing & (1ULL << square)) return 'K';

    return '.';
}

void Board::print_bitboard(){
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank+1 << "  ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << piece_at_square(square) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   A B C D E F G H\n";
}