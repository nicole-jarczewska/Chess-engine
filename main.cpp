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

#include "moves.hpp"
#include "moves.cpp"

#include "board.hpp"
#include "board.cpp"



int main() {
    Board board;
    Moves moves;
    board.print_bitboard();
    PawnMoves pawnMoves = moves.pawn(board.whitePawn, board.boardWhite, board.boardBlack, 1);
    board.whitePawn = pawnMoves.forward | pawnMoves.doubleForward | pawnMoves.leftCapture | pawnMoves.rightCapture;
    board.print_bitboard();

    // board.whiteKnight = moves.knight(board.whiteKnight);
    // std::cout << board.whiteKnight << std::endl; 

    return 0;
}