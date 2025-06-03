#include "include/moves.hpp"

uint64_t Moves::kingMoves(uint64_t king, uint64_t board, uint64_t boardEnemy){
    if (king == 0) return 0;
    uint64_t moves = 0ULL;

    // Vertical
    moves |= (king << 8);   // up
    moves |= (king >> 8);   // down

    // Horizontal
    moves |= (king << 1) & ~A_vertical;  // right
    moves |= (king >> 1) & ~H_vertical;  // left

    // Diagonals
    moves |= (king << 9) & ~A_vertical;  // up-right
    moves |= (king << 7) & ~H_vertical;  // up-left
    moves |= (king >> 7) & ~A_vertical;  // down-right
    moves |= (king >> 9) & ~H_vertical;  // down-left

    moves &= ~board;

    return moves;
}

uint64_t Moves::queenMoves(uint64_t queen, uint64_t board, uint64_t boardEnemy){
    if (queen == 0) return 0;
    return Moves::rookMoves(queen, board, boardEnemy) | Moves::bishopMoves(queen, board, boardEnemy);
}

uint64_t Moves::rookMoves(uint64_t rook, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;

    // int at_square = ctzll(rook) + 1;
    // uint64_t square = 1ULL << at_square;

    // up
    for (uint64_t potencial_move = rook << 8; potencial_move != 0 && ((potencial_move >> 8) & eight_horizontal) == 0; potencial_move <<= 8) {
        if (board & potencial_move) break;
        moves |= potencial_move;
        if (boardEnemy & potencial_move) break;
    }

    // down
    for (uint64_t potencial_move = rook >> 8; potencial_move != 0&& ((potencial_move << 8) & one_horizontal) == 0; potencial_move >>= 8) {
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
        int at_square = ctzll(rooks);
        uint64_t square = 1ULL << at_square;
        moves |= rookMoves(square, board, boardEnemy);
        rooks &= rooks - 1;
    }
    return moves;
}

uint64_t Moves::bishopMoves(uint64_t bishop, uint64_t board, uint64_t boardEnemy){
    uint64_t moves = 0ULL;
    // int at_square = ctzll(bishop) + 1;
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
        int at_square = ctzll(bishops);
        uint64_t square = 1ULL << at_square;
        move |= bishopMoves(square, board, boardEnemy);
        bishops &= bishops - 1;
    }
    return move;
}

uint64_t Moves::knightMoves(uint64_t knight, uint64_t board, uint64_t boardEnemy) {
    if (knight == 0) return 0;
    uint64_t l1 = ~A_vertical; // ~A file
    uint64_t l2 = ~(A_vertical&B_vertical); // ~A&B files
    uint64_t r1 = ~H_vertical; // ~H file
    uint64_t r2 = ~(H_vertical&G_vertical); // ~G&H files

    uint64_t moves = 0ULL;

    moves |= (knight << 17) & r2;
    moves |= (knight << 15) & l1;
    moves |= (knight << 10) & r2;
    moves |= (knight << 6)  & l2;
    moves |= (knight >> 17) & l1;
    moves |= (knight >> 15) & r1;
    moves |= (knight >> 10) & l2;
    moves |= (knight >> 6)  & r2;

    // Remove your own pieces
    moves &= ~board;

    return moves;
}

uint64_t Moves::pawnMoves(uint64_t pawn, uint64_t board, uint64_t boardEnemy, int color) {
    if (pawn == 0) return 0;
     uint64_t forward = 0ULL, doubleForward = 0ULL, leftCapture = 0ULL, rightCapture = 0ULL;
     uint64_t empty = ~(board | boardEnemy);

        if (color == 1) {
        forward = (pawn << 8) & empty;
        doubleForward = ((pawn & two_horizontal) << 16) & empty & (empty << 8);
        leftCapture  = ((pawn & ~A_vertical) << 7) & boardEnemy;
        rightCapture = ((pawn & ~H_vertical) << 9) & boardEnemy;
    } else {
        forward = (pawn >> 8) & empty;
        doubleForward = ((pawn & seven_horizontal) >> 16) & empty & (empty >> 8);
        leftCapture  = ((pawn & ~H_vertical) >> 9) & boardEnemy;
        rightCapture = ((pawn & ~A_vertical) >> 7) & boardEnemy;
    }

    return forward | doubleForward | leftCapture | rightCapture;
}

// board.whitePawn &= ~board.set_square(12);  // Remove from e2
// board.whitePawn |= board.set_square(28);   // Move to e4

// board.board &= ~board.set_square(12);
// board.board |= board.set_square(28);
