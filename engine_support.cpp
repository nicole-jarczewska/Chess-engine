#include "include/engine.hpp"

Moves MOVES;
Board BOARD;

std::vector<std::pair<std::string, uint64_t*>> piecesWhite = {
    {"king", &BOARD.whiteKing},
    {"queen", &BOARD.whiteQueen},
    {"rook", &BOARD.whiteRook},
    {"bishop", &BOARD.whiteBishop},
    {"knight", &BOARD.whiteKnight},
    {"pawn", &BOARD.whitePawn}
};
std::vector<std::pair<std::string, uint64_t*>> piecesBlack = {
    {"king", &BOARD.blackKing},
    {"queen", &BOARD.blackQueen},
    {"rook", &BOARD.blackRook},
    {"bishop", &BOARD.blackBishop},
    {"knight", &BOARD.blackKnight},
    {"pawn", &BOARD.blackPawn}
};

void setGame(int& gameState, int& turn, int COLOR, std::vector<std::pair<std::string, uint64_t*>>& pieces,
std::vector<std::pair<std::string, uint64_t*>>& piecesEnemy) {
    gameState = 0;

    if (COLOR == 1) {
        pieces = piecesWhite;
        piecesEnemy = piecesBlack;
        turn = 1;
    } else {
        pieces = piecesBlack;
        piecesEnemy = piecesWhite;
        turn = -1;
    }
}

std::vector<std::pair<std::string, uint64_t>> dereferencePieces(const std::vector<std::pair<std::string, uint64_t*>>& piecesPtr) {
    std::vector<std::pair<std::string, uint64_t>> piecesCopy;
    for (const auto& [type, bbPtr] : piecesPtr) {
        piecesCopy.emplace_back(type, *bbPtr);
    }
    return piecesCopy;
}

uint64_t getPieceBitboard(const std::vector<std::pair<std::string, uint64_t>>& pieces, const std::string& pieceName) {
    for (const auto& [type, bitboard] : pieces) {
        if (type == pieceName) {
            return bitboard;
        }
    }
    return 0;
}

uint64_t countBoard(const std::vector<std::pair<std::string, uint64_t>>& pieces) {
    uint64_t board = 0;
    for (auto& [_, bb] : pieces) board |= bb;
    return board;
}

uint64_t iterateMoves(uint64_t piece, uint64_t board, uint64_t boardEnemy, int color, const std::string& type) {
    using MoveGenFn = std::function<uint64_t(uint64_t, uint64_t, uint64_t)>;

    static std::unordered_map<std::string, MoveGenFn> moveGenerators = {
        {"knight", [&](uint64_t piece, uint64_t board, uint64_t boardEnemy) {
            return MOVES.allKnightsMoves(piece, board, boardEnemy);
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

    if (type == "pawn") return MOVES.pawnMoves(piece, board, boardEnemy, color);

    return 0;
}

int pieceValue(const std::string& type) {
    static const std::unordered_map<std::string, int> pieceValue = {
        {"pawn", 1},
        {"knight", 3},
        {"bishop", 3},
        {"rook", 5},
        {"queen", 9},
        {"king", 10000}
    };

    auto it = pieceValue.find(type);
    if (it != pieceValue.end()) {
        return it->second;
    }
    return 0; // Unknown piece
}

int initialCheck(int turn, int color, // color: 1 for white, -1 for black
std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) { 
    uint64_t board = countBoard(pieces);
    uint64_t boardEnemy = countBoard(piecesEnemy);

    uint64_t potentialEnemyMoves = 0ULL;
    uint64_t king = getPieceBitboard(pieces, "king");

    for (const auto& [type, bitboard] : piecesEnemy) {
        if (bitboard == 0) continue;
        potentialEnemyMoves |= iterateMoves(bitboard, boardEnemy, board, -color, type);
    }

    if (MOVES.isCheck(king, potentialEnemyMoves)) {
        if (MOVES.isCheckmate(king, board, boardEnemy, potentialEnemyMoves, color, pieces, piecesEnemy)) {
            return (turn == 1) ? 3 : 2;  // 2: player wins, 3: enemy wins
        }
        return 1;  // in check
    }

    return 0; // game continues
}