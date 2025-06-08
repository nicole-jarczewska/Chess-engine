#include "moves.hpp"


bool isSlidingPiece(uint64_t square, const std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) {
    for (const auto& [type, bitboard] : piecesEnemy) {
        if ((bitboard & square) != 0ULL) {
            return (type == "bishop" || type == "rook" || type == "queen");
        }
    }
    return false;
}

uint64_t squaresBetween(uint64_t a, uint64_t b) {
    if (a == b) return 0ULL;

    int from = __builtin_ctzll(a);
    int to = __builtin_ctzll(b);

    uint64_t between = 0ULL;

    int df = (to % 8) - (from % 8);
    int dr = (to / 8) - (from / 8);

    int step;

    if (df == 0 && dr != 0) step = 8;
    else if (dr == 0 && df != 0) step = 1;
    else if (abs(df) == abs(dr)) step = (df > 0 ? 1 : -1) + (dr > 0 ? 8 : -8);
    else return 0ULL;

    int pos = from + step;
    while (pos != to) {
        between |= 1ULL << pos;
        pos += step;
    }

    return between;
}

bool Moves::isCheck(uint64_t king, uint64_t moveEnemy) {
    return (king & moveEnemy) != 0;
}

bool Moves::isCheckmate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy, int color,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy){
    uint64_t kingPotentialMoves = kingMoves(king, board, boardEnemy);
    if((kingPotentialMoves & ~movesEnemy) != 0ULL) return false; // king is not in check


    std::vector<uint64_t> checkingPieces;
    for (const auto& [type, bitboard] : piecesEnemy) {
        if (bitboard == 0) continue;
        uint64_t bitboardCopy = bitboard;
        while(bitboardCopy) {
            uint64_t attacker = 1ULL << __builtin_ctzll(bitboardCopy);

            uint64_t attackMask = iterateMoves(attacker, boardEnemy, board, -color, type);
            if ((attackMask & king) != 0ULL) {
                checkingPieces.push_back(attacker);
            }
            bitboardCopy &= (bitboardCopy - 1); 
        }
    }

    if (checkingPieces.size() >= 2) return true;
    

    uint64_t checker = checkingPieces[0];

    uint64_t blockMask = 0ULL;
    if (isSlidingPiece(checker, piecesEnemy)) {
        blockMask = squaresBetween(king, checker);
    }

    for (auto& [type, bitboard] : pieces) {
        if (bitboard == 0) continue;

        uint64_t bitboardCopy = bitboard;
        while(bitboardCopy) {
            uint64_t from = 1ULL << __builtin_ctzll(bitboardCopy);

            uint64_t moves = iterateMoves(from, boardEnemy, board, color, type);

            if ((moves & checker) != 0ULL || (moves & blockMask) != 0ULL) {
                return false; 
            }
            bitboardCopy &= (bitboardCopy - 1); 
        }
    }

    return true;
}