﻿#include "moves.hpp"

uint64_t Moves::kingMoves(uint64_t king, uint64_t board, uint64_t boardEnemy){
    if (king == 0) return 0;
    uint64_t moves = 0ULL;

    moves |= (king & ~eight_horizontal) << 8;   // up
    moves |= (king & ~one_horizontal) >> 8;   // down

    moves |= (king & ~H_vertical) << 1;  // right
    moves |= (king & ~A_vertical) >> 1;  // left

    moves |= (king & ~H_vertical) << 9;  // up-right
    moves |= (king & ~A_vertical) << 7;  // up-left
    moves |= (king & ~A_vertical) >> 9;  // down-right
    moves |= (king & ~H_vertical) >> 7;  // down-left

    moves &= ~board;

    return moves;
}

uint64_t Moves::queenMoves(uint64_t queen, uint64_t board, uint64_t boardEnemy){
    if (queen == 0) return 0;
    return Moves::allRooksMoves(queen, board, boardEnemy) | Moves::allBishopsMoves(queen, board, boardEnemy);
}

uint64_t Moves::rookMoves(uint64_t rook, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;

    // int at_square = __builtin_ctzll(rook) + 1;
    // uint64_t square = 1ULL << at_square;

    // up
    for (uint64_t potencial_move = rook << 8; potencial_move != 0 && ((potencial_move >> 8) & eight_horizontal) == 0; potencial_move <<= 8) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down
    for (uint64_t potencial_move = rook >> 8; potencial_move != 0 && ((potencial_move << 8) & one_horizontal) == 0; potencial_move >>= 8) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // right
    for (uint64_t potencial_move = rook << 1; potencial_move != 0 && ((potencial_move >> 1) & H_vertical) == 0; potencial_move <<= 1) {
        if ((potencial_move & H_vertical) == H_vertical) break;
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // left
    for (uint64_t potencial_move = rook >> 1; potencial_move != 0 && ((potencial_move << 1) & A_vertical) == 0; potencial_move >>= 1) {
        if ((potencial_move & A_vertical) == A_vertical) break;
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }
    moves &= ~board;
    return moves;
}

uint64_t Moves::allRooksMoves(uint64_t rooks, uint64_t board, uint64_t boardEnemy) {
    if (rooks == 0) return 0;
    uint64_t moves = 0ULL;
    while (rooks) {
        int at_square = __builtin_ctzll(rooks);
        uint64_t square = 1ULL << at_square;
        moves |= rookMoves(square, board, boardEnemy);
        rooks &= rooks - 1;
    }
    return moves;
}

uint64_t Moves::bishopMoves(uint64_t bishop, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;
    // int at_square = __builtin_ctzll(bishop) + 1;
    // uint64_t square = 1ULL << at_square;

    //up right
    for (uint64_t potencial_move = bishop << 9; potencial_move != 0 && ((potencial_move >> 9) & eight_horizontal) == 0  && ((potencial_move >> 9) & H_vertical) == 0; potencial_move <<= 9) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    //up left
    for (uint64_t potencial_move = bishop << 7; potencial_move != 0 && ((potencial_move >> 7) & eight_horizontal) == 0  && ((potencial_move >> 7) & A_vertical) == 0; potencial_move <<= 7) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down left
    for (uint64_t potencial_move = bishop >> 9; potencial_move != 0 && ((potencial_move << 9) & one_horizontal) == 0  && ((potencial_move << 9) & A_vertical) == 0; potencial_move >>= 9) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down right
    for (uint64_t potencial_move = bishop >> 7; potencial_move != 0 && ((potencial_move << 7) & one_horizontal) == 0  && ((potencial_move << 7) & H_vertical) == 0; potencial_move >>= 7) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }
    moves &= ~board;
    return moves;
}

uint64_t Moves::allBishopsMoves(uint64_t bishops, uint64_t board, uint64_t boardEnemy) {
    if (bishops == 0) return 0;
    uint64_t move = 0ULL;
    while (bishops) {
        int at_square = __builtin_ctzll(bishops);
        uint64_t square = 1ULL << at_square;
        move |= bishopMoves(square, board, boardEnemy);
        bishops &= bishops - 1;
    }
    return move;
}

uint64_t Moves::knightMoves(uint64_t knight, uint64_t board, uint64_t boardEnemy) {
    if (knight == 0) return 0;
    uint64_t moves = 0ULL;


    moves |= (knight & ~(eight_horizontal|seven_horizontal|A_vertical)) << 15; //up left
    moves |= (knight & ~(eight_horizontal|seven_horizontal|H_vertical)) << 17; // up right

    moves |= (knight & ~(one_horizontal|two_horizontal|H_vertical)) >> 15; // down right
    moves |= (knight & ~(one_horizontal|two_horizontal|A_vertical)) >> 17;  // down left

    moves |= (knight & ~(eight_horizontal|B_vertical|A_vertical)) << 6; // left up
    moves |= (knight & ~(eight_horizontal|G_vertical|H_vertical)) << 10; // right up

    moves |= (knight & ~(one_horizontal|G_vertical|H_vertical)) >> 6; // right down
    moves |= (knight & ~(one_horizontal|B_vertical|A_vertical)) >> 10; // left down
    
    moves &= ~board;

    return moves;
}

uint64_t Moves::allKnightsMoves(uint64_t knights, uint64_t board, uint64_t boardEnemy) {
    if (knights == 0) return 0;
    uint64_t move = 0ULL;
    while (knights) {
        int at_square = __builtin_ctzll(knights);
        uint64_t square = 1ULL << at_square;
        move |= knightMoves(square, board, boardEnemy);
        knights &= knights - 1;
    }
    return move;
}

uint64_t Moves::pawnMoves(uint64_t pawn, uint64_t board, uint64_t boardEnemy, int color) {
    if (pawn == 0) return 0;
     uint64_t forward = 0ULL, doubleForward = 0ULL, leftCapture = 0ULL, rightCapture = 0ULL;
     uint64_t empty = ~(board | boardEnemy);

        if (color == 1) {
        forward = (pawn << 8) & empty;
        doubleForward = ((pawn & two_horizontal) << 16) & empty & (empty << 8);
        leftCapture  = ((pawn & ~H_vertical) << 9) & boardEnemy;
        rightCapture = ((pawn & ~A_vertical) << 7) & boardEnemy;
    } else {
        forward = (pawn >> 8) & empty;
        doubleForward = ((pawn & seven_horizontal) >> 16) & empty & (empty >> 8);
        leftCapture  = ((pawn & ~A_vertical) >> 9) & boardEnemy;
        rightCapture = ((pawn & ~H_vertical) >> 7) & boardEnemy;
    }

    return forward | doubleForward | leftCapture | rightCapture;
}