#pragma once

class Board{
public:
    Board();

    uint64_t set_square(int squere);
    bool piece_present(uint64_t board, int squere);
    char piece_at_square(int square);

    void print(){
        for(int i = 0; i < 64; ++i) {
            std::cout << visual_board[i] << " ";
            if ((i + 1) % 8 == 0) std::cout << std::endl;
        }
    };

    void print_bitboard();

    
    int visual_board[64];

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