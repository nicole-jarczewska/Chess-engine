#pragma once
#include "libraries.hpp"
#include "moves.hpp"
#include "board.hpp"
#include "bit_support.hpp"

struct Move {
    std::string pieceType;
    uint64_t from;
    uint64_t to;
};

extern Moves MOVES;
extern Board BOARD;
extern std::vector<std::pair<std::string, uint64_t*>> piecesBlack;
extern std::vector<std::pair<std::string, uint64_t*>> piecesWhite;

void setGame(int& gameState, int& turn, int COLOR, std::vector<std::pair<std::string, uint64_t*>>& pieces,
std::vector<std::pair<std::string, uint64_t*>>& piecesEnemy);

std::vector<std::pair<std::string, uint64_t>> dereferencePieces(const std::vector<std::pair<std::string, uint64_t*>>& piecesPtr);

uint64_t getPieceBitboard(const std::vector<std::pair<std::string, uint64_t>>& pieces, const std::string& pieceName);

uint64_t iterateMoves(uint64_t piece, uint64_t board, uint64_t boardEnemy, int color, const std::string& type);

int initialCheck(int turn, uint64_t board, uint64_t boardEnemy, int color,
std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy);

int pieceValue(const std::string& type);

int evaluate(int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy);

void applyMove(int color, std::vector<std::pair<std::string, uint64_t*>>& pieces, std::vector<std::pair<std::string, uint64_t*>>& piecesEnemy, const Move& move);

void applyMoveMM(int color, std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy, const Move& move);

std::vector<Move> generateAllMoves(const std::vector<std::pair<std::string, uint64_t>>& pieces, uint64_t board, uint64_t boardEnemy, int color);

int minMax(int depth, int fullDepth, int turn, int color, uint64_t board, uint64_t boardEnemy,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy, int alpha, int beta);

Move findBestMove(int turn, int depth, int color, std::vector<std::pair<std::string, uint64_t>> pieces,
std::vector<std::pair<std::string, uint64_t>> piecesEnemy);