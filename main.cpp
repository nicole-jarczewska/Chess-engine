#include <stdlib.h>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <optional>
#include <ctime>
#include <chrono>
#include <queue>
#include <random>

#include "include/moves.hpp"
#include "moves.cpp"

#include "include/board.hpp"
#include "board.cpp"



int main() {
    Board board;
    Moves moves;
    board.print_bitboard();
    board.blackRook = moves.bishop(board.blackRook, 0ULL, board.blackKing);
    

    // board.blackRook = board.blackRook >> 3;
    board.print_bitboard();

    std::cout << "Piece at square 12: " << board.piece_at_square(1) << std::endl;

    // board.whiteKnight = moves.knight(board.whiteKnight);
    // std::cout << board.whiteKnight << std::endl; 

    return 0;
}