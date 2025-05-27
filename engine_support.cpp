#include "include/engine.hpp"

#include "moves.cpp"
#include "special.cpp"
#include "board.cpp"

void setGame(int& gameState, int& turn, int COLOR, std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) {
    gameState = 0; // playing

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


int initialCheck(int turn, uint64_t board, uint64_t boardEnemy, int color, // color: 1 for white, -1 for black
std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) { 
    uint64_t potentialEnemyMoves = 0ULL;
    uint64_t king = getPieceBitboard(pieces, "king");

    for (const auto& [type, bitboard] : piecesEnemy) {
        if (bitboard == 0) continue;
        potentialEnemyMoves |= iterateMoves(bitboard, boardEnemy, board, color, type);
    }

    if (MOVES.isCheck(king, potentialEnemyMoves)) {
        if (MOVES.isCheckmate(king, board, boardEnemy, potentialEnemyMoves, color, pieces, piecesEnemy)) {
            return (turn == 1) ? 2 : 3;  // 2: player wins, 3: enemy wins
        }
        return 1;  // in check
    }

    // else if (MOVES.isStalemate(king, board, boardEnemy, potentialEnemyMoves)) {
    //     std::cout << "stalemate" << std::endl;
    //     return 4; // draw
    // }

    return 0; // game continues
}

int pieceValue(const std::string& type) {
    static const std::unordered_map<std::string, int> pieceValue = {
        {"pawn", 1},
        {"knight", 3},
        {"bishop", 3},
        {"rook", 5},
        {"queen", 9},
        {"king", 10000} // Typically assigned a very high value
    };

    auto it = pieceValue.find(type);
    if (it != pieceValue.end()) {
        return it->second;
    }
    return 0; // Unknown piece
}

int evaluate(std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) { //int turn, uint64_t move, int color
    // int eval = 0;

    // for (const auto& [type, bitboard] : piecesEnemy) {
    //     if (bitboard == 0) continue;
    //     if(move&bitboard){
    //         eval = pieceValue(type); // enemy piece captured
    //         if(type == "pawn"); // ?????????
    //         break;
    //     }
    // }

    // if(move&0xFF818181818181FFULL) eval *= 0.7; // bad move, piece at the edge
    // else if(move&0x7E7E7E7E7E7E00ULL) eval *= 0.9; // closer to the cente
    // else eval *= 1.5; // taking centre of the board - good move

    // return eval;

    int score = 0;
    for (const auto& [type, bitboard] : pieces)
        score += __builtin_popcountll(bitboard) * pieceValue(type);

    for (const auto& [type, bitboard] : piecesEnemy)
        score -= __builtin_popcountll(bitboard) * pieceValue(type);

    return score;
}

void applyMove(std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy, const Move& move) {
    for (auto& [type, bitboard] : pieces) {
        if (bitboard & move.from) {
            bitboard &= ~move.from;
            bitboard |= move.to;
            break;
        }
    }

    // capturing
    for (auto& [type, bitboard] : piecesEnemy) {
        if (bitboard & move.to) {
            bitboard &= ~move.to;
            break;
        }
    }
}

std::vector<Move> generateAllMoves(const std::vector<std::pair<std::string, uint64_t>>& pieces, uint64_t board, uint64_t boardEnemy, int color) {
    std::vector<Move> moves;

    for (const auto& [type, bitboard] : pieces) {
        if (bitboard == 0) continue;
        auto bitboardCopy = bitboard;

        while (bitboardCopy) {
            uint64_t onePiece = bitboardCopy & -bitboardCopy; // isolate LSB
            uint64_t moveMap = iterateMoves(onePiece, board, boardEnemy, color, type);
            
            while (moveMap) {
                uint64_t to = moveMap & -moveMap;
                moves.push_back({type, to, onePiece});
                moveMap &= (moveMap - 1);
            }

            bitboardCopy &= (bitboardCopy - 1);
        }
    }

    //     // Get individual piece positions
    //     for (int i = 0; i < 64; ++i) {
    //         uint64_t mask = 1ULL << i;
    //         if (bitboard & mask) {
    //             uint64_t moveMap = iterateMoves(mask, board, boardEnemy, color, type);
    //             while (moveMap) {
    //                 uint64_t to = moveMap & -moveMap;
    //                 moveMap &= (moveMap - 1);
    //                 moves.push_back({type, i, to});
    //             }
    //         }
    //     }
    // } gówno z chata

    return moves;
}

int minMax(int depth, int turn, int color, uint64_t board, uint64_t boardEnemy,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) {
    if (depth == 0) return evaluate(pieces, piecesEnemy);

    int bestEval = turn ? -99999 : 99999;

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);

    for (const Move& move : moves) {
        // Make a copy of pieces to simulate the move
        auto piecesCopy = pieces;
        auto piecesEnemyCopy = piecesEnemy;

        applyMove(piecesCopy, piecesEnemyCopy, move);

        uint64_t boardCopy = 0, boardEnemyCopy = 0;
        for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
        for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

        int eval = minMax( depth - 1, turn*(-1), color*(-1), boardCopy, boardEnemyCopy, piecesEnemyCopy, piecesCopy);

        if (turn) bestEval = std::max(bestEval, eval);
        else bestEval = std::min(bestEval, eval);
    }

    return bestEval;
}

Move findBestMove(int turn, int depth, int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) {
    // auto myPieces = (color == 1) ? piecesWhite : piecesBlack;
    // auto enemyPieces = (color == 1) ? piecesBlack : piecesWhite;

    uint64_t board = 0, boardEnemy= 0;
    for (auto& [_, bb] : pieces) board |= bb;
    for (auto& [_, bb] : piecesEnemy) boardEnemy |= bb;

    Move bestMove;
    int bestEval = -99999;

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);

    for (const Move& move : moves) {
        auto piecesCopy = pieces;
        auto piecesEnemyCopy = piecesEnemy;
        applyMove(piecesCopy, piecesEnemyCopy, move);

        uint64_t boardCopy = 0, boardEnemyCopy = 0;
        for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
        for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

        int eval = minMax(depth - 1, turn, color*(-1), boardEnemyCopy, boardCopy,piecesEnemyCopy, piecesCopy);

        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        }
    }

    return bestMove;
}