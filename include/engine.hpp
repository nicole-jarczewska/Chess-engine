#pragma once
#include "libraries.hpp"
#include "moves.hpp"
#include "board.hpp"

struct Move {
    std::string pieceType;
    uint64_t from;
    uint64_t to;
};

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

uint64_t getPieceBitboard(const std::vector<std::pair<std::string, uint64_t>>& pieces, const std::string& pieceName);

uint64_t iterateMoves(uint64_t piece, uint64_t board, uint64_t boardEnemy, int color, const std::string& type);

int initialCheck(int turn, uint64_t board, uint64_t boardEnemy, int color);

void setGame(int& gameState, int& turn, int COLOR);

int pieceValue(const std::string& type);

int evaluate();

void applyMove(std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy, const Move& move);

std::vector<Move> generateAllMoves(const std::vector<std::pair<std::string, uint64_t>>& pieces, uint64_t board, uint64_t boardEnemy, int color);

int minMax(int depth, bool turn, int color, uint64_t board, uint64_t boardEnemy,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy);

Move findBestMove(int turn, int depth, int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy);
