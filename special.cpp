#include "include/moves.hpp"

bool Moves::isCheck(uint64_t king, uint64_t moveEnemy) {
    return (king & moveEnemy) != 0;
}

bool Moves::isCheckmate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy){
    uint64_t kingPotencialMoves = kingMoves(king, board, boardEnemy & movesEnemy);
    if ((kingPotencialMoves & movesEnemy) == 0) {
        return true;
    }

    return false;
}

bool Moves::isStalemate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy){
    uint64_t kingPotencialMoves = kingMoves(king, boardEnemy, movesEnemy);
    if ((kingPotencialMoves & movesEnemy) == 0) {
        return true;
    }

    return false;
}

