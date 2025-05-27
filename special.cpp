#include "include/moves.hpp"


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

    int from = __builtin_ctzll(a); // index of LSB
    int to = __builtin_ctzll(b);

    uint64_t between = 0ULL;

    int df = (to % 8) - (from % 8);
    int dr = (to / 8) - (from / 8);

    int step;

    // Determine the direction
    if (df == 0 && dr != 0) step = 8;              // same file
    else if (dr == 0 && df != 0) step = 1;          // same rank
    else if (abs(df) == abs(dr)) step = (df > 0 ? 1 : -1) + (dr > 0 ? 8 : -8); // diagonal
    else return 0ULL; // not aligned

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
    uint64_t kingPotentialMoves = kingMoves(king, board, boardEnemy | movesEnemy);
    if ((kingPotentialMoves & ~movesEnemy) != 0) {
        return false;
    }

    // // all legal moves for allied pieces
    // for (auto& [type, bitboard] : pieces) {
    //     if (bitboard == 0) continue;
    //     potentialMoves |= iterateMoves(bitboard, boardEnemy, board, color, type);
    // }

    // if(potentialMoves & boardEnemy != 0ULL){
    //    for (const auto& [type, bitboard] : piecesEnemy) {
    //         if (bitboard == 0) continue;
    //         potentialEnemyMoves |= iterateMoves(bitboard, boardEnemy, board, color, type);
    //     }
    // }
       
    // return true; //checkmate

    // Step 2: Identify checking piece(s)
    std::vector<uint64_t> checkingPieces;
    for (const auto& [type, bitboard] : piecesEnemy) {
        if (bitboard == 0) continue;

        for (int i = 0; i < 64; ++i) {
            uint64_t attacker = 1ULL << i;
            if ((bitboard & attacker) == 0) continue;

            uint64_t attackMask = iterateMoves(attacker, boardEnemy, board, -color, type);
            if ((attackMask & king) != 0ULL) {
                checkingPieces.push_back(attacker);
            }
        }
    }

    if (checkingPieces.size() >= 2) return true;
    

    // Step 3: Try to capture the checking piece or block its path
    uint64_t checker = checkingPieces[0];

    // Find the path between king and checker if it's a sliding piece
    uint64_t blockMask = 0ULL;
    if (isSlidingPiece(checker, piecesEnemy)) {
        blockMask = squaresBetween(king, checker);
    }

    // Step 4: Can any allied piece capture the checker or block?
    for (auto& [type, bitboard] : pieces) {
        if (bitboard == 0) continue;

        for (int i = 0; i < 64; ++i) {
            uint64_t from = 1ULL << i;
            if ((bitboard & from) == 0) continue;

            uint64_t moves = iterateMoves(from, boardEnemy, board, color, type);

            // Can this piece capture the checker or block?
            if ((moves & checker) != 0ULL || (moves & blockMask) != 0ULL) {
                return false; // Escape is possible
            }
        }
    }

    return true; // No escape
}

bool Moves::isStalemate(uint64_t king, uint64_t board, uint64_t boardEnemy,  uint64_t movesEnemy){
    uint64_t kingPotencialMoves = kingMoves(king, boardEnemy, movesEnemy);
    if ((kingPotencialMoves & movesEnemy) == 0) {
        return true;
    }

    return false;
}