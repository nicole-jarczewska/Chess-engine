#include "include/libraries.hpp"

#include "include/moves.hpp"
#include "moves.cpp"
#include "special.cpp"

#include "include/board.hpp"
#include "board.cpp"

Moves MOVES;
Board BOARD;

std::vector<std::pair<std::string, uint64_t>> piecesWhite = {
    {"king", BOARD.whiteKing},
    {"queen", BOARD.whiteQueen},
    {"rook", BOARD.whiteRook},
    {"bishop", BOARD.whiteBishop},
    {"knight", BOARD.whiteKnight},
    {"pawn", BOARD.whitePawn}
};

std::vector<std::pair<std::string, uint64_t>> piecesBlack = {
    {"king", BOARD.blackKing},
    {"queen", BOARD.blackQueen},
    {"rook", BOARD.blackRook},
    {"bishop", BOARD.blackBishop},
    {"knight", BOARD.blackKnight},
    {"pawn", BOARD.blackPawn}
};

uint64_t getPieceBitboard(const std::vector<std::pair<std::string, uint64_t>>& pieces, const std::string& pieceName) {
    for (const auto& [type, bitboard] : pieces) {
        if (type == pieceName) {
            return bitboard;
        }
    }
    return 0;
}

uint64_t iterateMoves(uint64_t piece, uint64_t board, uint64_t boardEnemy, int color, const std::string& type) {
    if (type == "pawn") return MOVES.pawnMoves(piece, board, boardEnemy, color);

    using MoveGenFn = std::function<uint64_t(uint64_t, uint64_t, uint64_t)>;

    static std::unordered_map<std::string, MoveGenFn> moveGenerators = {
        {"knight", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.knightMoves(piece, board, boardEnemy);
        }},
        {"rook", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.allRooksMoves(piece, board, boardEnemy);
        }},
        {"bishop", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.allBishopsMoves(piece, board, boardEnemy);
        }},
        {"queen", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.queenMoves(piece, board, boardEnemy);
        }},
        {"king", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.kingMoves(piece, board, boardEnemy);
        }}
    };

    auto it = moveGenerators.find(type);
    if (it != moveGenerators.end()) {
        return it->second(piece, board, boardEnemy);
    }

    return 0;
}


int initialCheck(int turn, uint64_t board, uint64_t boardEnemy, int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) { // 1 for white, -1 for black
    uint64_t potentialEnemyMoves = 0ULL;
    uint64_t king = getPieceBitboard(pieces, "king");

    for (const auto& [type, bitboard] : piecesEnemy) {
        if (bitboard == 0) continue;
        potentialEnemyMoves |= iterateMoves(bitboard, boardEnemy, board, color, type); // board & boardEnemy switched since we check the moves for the enemy
    }

    if (MOVES.isCheck(king, potentialEnemyMoves)) {
        if (MOVES.isCheckmate(king, board, boardEnemy, potentialEnemyMoves)) {
            if(turn == 1) return 2; // victory
            else if(turn == -1) return 3; // enemy victory
        }
        return 1; // check(for the current player)
    }
    else if (MOVES.isStalemate(king, board, boardEnemy, potentialEnemyMoves)) {
        std::cout << "stalemate" << std::endl;
        return 4; // draw
    }

    return 0; // game continues
}