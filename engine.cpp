#include "include/engine.hpp"
#include "engine_support.cpp"

#include "moves.cpp"
#include "checkmate.cpp"
#include "board.cpp"

std::vector<Move> validMoves(uint64_t piece, int color,std::vector<std::pair<std::string, uint64_t>>& pieces, 
std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) {
    std::vector<Move> moves;
    std::string pieceType;

    uint64_t board = countBoard(pieces);
    uint64_t boardEnemy = countBoard(piecesEnemy);

    for (auto& [type, bitboard] : pieces) {
        if (bitboard & piece) {
            pieceType = type;
            break;
        }
    }

    auto bitboardCopy = piece;

    while (bitboardCopy) {
        uint64_t onePiece = 1ULL << __builtin_ctzll(bitboardCopy);
        uint64_t moveMap = iterateMoves(onePiece, board, boardEnemy, color, pieceType);
            
        while (moveMap) {
                uint64_t to = 1ULL << __builtin_ctzll(moveMap);

                auto piecesCopy = pieces;
                auto piecesEnemyCopy = piecesEnemy;

                Move candidateMove{pieceType, onePiece, to};
                applyMoveMM(color, piecesCopy, piecesEnemyCopy, candidateMove);

                uint64_t boardCopy = 0, boardEnemyCopy = 0;
                for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
                for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

                if (initialCheck(1, color, piecesCopy, piecesEnemyCopy) == 0)
                    moves.push_back(candidateMove);

                moveMap &= (moveMap - 1);
        }

        bitboardCopy &= (bitboardCopy - 1);
    }

    return moves;
}

int evaluate(int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) { //int turn, uint64_t move, int color
    int score = 0;
    for (const auto& [type, bitboard] : pieces){
        score += __builtin_popcountll(bitboard) * pieceValue(type);
    //     if(type == "pawn"){
    //     if (color == 1) score += __builtin_popcountll(bitboard & 0x00FF000000000000ULL) * 3; // white pawns about to be promoted
    //     else score += __builtin_popcountll(bitboard & 0x000000000000FF00ULL) * 3; // black pawns about to be promoted
    //     }
    }

    for (const auto& [type, bitboard] : piecesEnemy){
        score -= __builtin_popcountll(bitboard) * pieceValue(type);
        // if(type == "pawn"){
        // if (color == 1) score -= __builtin_popcountll(bitboard & 0x00FF000000000000ULL) * 3; // white pawns about to be promoted
        // else score -= __builtin_popcountll(bitboard & 0x000000000000FF00ULL) * 3; // black pawns about to be promoted
        // }
    }
    

    // extra points for figures near centre // no pawn so engine evoleves figures //only until 15 turns so engine 
    if(turnsTaken < 15) {
        uint64_t board = 0, boardEnemy = 0;
        for (auto& [type, bb] : pieces) 
            if(type != "pawn") board |= bb;
        for (auto& [type, bb] : piecesEnemy) 
            if(type != "pawn")boardEnemy |= bb;

        score += __builtin_popcountll(board & 0x7E7E7E7E7E7E00ULL) * 1; // near the center
        score -= __builtin_popcountll(boardEnemy & 0x7E7E7E7E7E7E00ULL) * 1; // enemy pieces near the center
        score += __builtin_popcountll(board & 0x3C3C3C3C0000ULL) * 3; // control of the center 
        score -= __builtin_popcountll(boardEnemy & 0x3C3C3C3C0000ULL) * 3; // enemy control of the center
    }
    

    uint64_t boardPawn = 0, boardEnemyPawn = 0;
    for (auto& [type, bb] : pieces) 
        if(type == "pawn") boardPawn |= bb;
    for (auto& [type, bb] : piecesEnemy) 
        if(type == "pawn") boardEnemyPawn |= bb;

    // score += __builtin_popcountll(boardPawn & 0x7E7E7E7E7E7E00ULL) * 1; // near the center
    // score -= __builtin_popcountll(boardEnemyPawn & 0x7E7E7E7E7E7E00ULL) * 1; // enemy pieces near the center
    score += __builtin_popcountll(boardPawn & 0x3C3C3C3C0000ULL) * 1; // control of the center 
    score -= __builtin_popcountll(boardEnemyPawn & 0x3C3C3C3C0000ULL) * 1; // enemy control of the center

    // if(initialCheck(1,-color, piecesEnemy, pieces) == 2) score = INT_MAX;

    if(getPieceBitboard(pieces, "king") == 0ULL) score = INT_MIN; // player lost
    if(getPieceBitboard(piecesEnemy, "king") == 0ULL) score = INT_MAX; // enemy lost

    return score;
}

void applyMove(int color, std::vector<std::pair<std::string, uint64_t*>>& pieces, std::vector<std::pair<std::string, uint64_t*>>& piecesEnemy, const Move& move) {
    for (auto& [type, bitboard] : pieces) {
        if ((*bitboard) & move.from) {
            (*bitboard) &= ~move.from; // remove piece

            if (type == "pawn" && ((color == 1 && (move.to & 0xFF00000000000000ULL)) || (color == -1 && (move.to & 0x00000000000000FFULL)))) { //promoting
                for (auto& [type2, bitboard2] : pieces) {
                    if (type2 == "queen") {
                        (*bitboard2) |= move.to; // adding queen
                        break;
                    }
                }
            }
            else (*bitboard) |= move.to; //regular move
            break;
        }
    }

    for (auto& [type, bitboard] : piecesEnemy) { //capture enemy 
        if ((*bitboard) & move.to) {
            (*bitboard) &= ~move.to;
            break;
        }
    }
}

void applyMoveMM(int color, std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy, const Move& move) { // for simulating moves
    for (auto& [type, bitboard] : pieces) {
        if ((bitboard) & move.from) {
            (bitboard) &= ~move.from; // remove piece

            if (type == "pawn" && ((color == 1 && (move.to & 0xFF00000000000000ULL)) || (color == -1 && (move.to & 0x00000000000000FFULL)))) { //promoting
                for (auto& [type2, bitboard2] : pieces) {
                    if (type2 == "queen") {
                        (bitboard2) |= move.to; // adding queen
                        break;
                    }
                }
            }
            else (bitboard) |= move.to; //regular move
            break;
        }
    }

    for (auto& [type, bitboard] : piecesEnemy) {
        if ((bitboard & move.to) != 0) {
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
            uint64_t onePiece = 1ULL << __builtin_ctzll(bitboardCopy);
            // uint64_t onePiece = bitboardCopy & -bitboardCopy; // isolate LSB
            uint64_t moveMap = iterateMoves(onePiece, board, boardEnemy, color, type);
            
            while (moveMap) {
                uint64_t to = 1ULL << __builtin_ctzll(moveMap);
                moves.push_back({type, onePiece, to});
                moveMap &= (moveMap - 1);
            }

            bitboardCopy &= (bitboardCopy - 1);
        }
    }
    return moves;
}

int minMax(int depth, int fullDepth, int turn, int color,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy, int alpha, int beta) {
    uint64_t board = countBoard(pieces);
    uint64_t boardEnemy = countBoard(piecesEnemy);

    if (depth == 0) {
        if (fullDepth % 2 == 0)
            return evaluate(color, pieces, piecesEnemy); // player turn
        else
            return evaluate(-color, piecesEnemy, pieces); // enemy turn
    }

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);
    int bestEval = (turn == -1) ? INT_MIN : INT_MAX;

    for (const Move& move : moves) {
            auto piecesCopy = pieces;
            auto piecesEnemyCopy = piecesEnemy;
            applyMoveMM(color, piecesCopy, piecesEnemyCopy, move);

            // if (initialCheck(turn, color, piecesCopy, piecesEnemyCopy) != 0) continue; // illegal moves

            int eval = minMax(depth - 1, fullDepth, -turn, -color, piecesEnemyCopy, piecesCopy, alpha, beta);

            if(turn == -1){
            bestEval = std::max(bestEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break; // beta cutoff
            }
            else{
                bestEval = std::min(bestEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha)
                break; // alpha cutoff
            }
        }

        // if(initialCheck(1, (turn == 1 ? color : color), (turn == 1 ? pieces : piecesEnemy), (turn == 1 ? piecesEnemy : pieces))==2)
        //     bestEval = INT_MAX;

    return bestEval;
}

Move findBestMove(int turn, int depth, int color, std::vector<std::pair<std::string, uint64_t>> pieces,
std::vector<std::pair<std::string, uint64_t>> piecesEnemy) {

    uint64_t board = countBoard(pieces);
    uint64_t boardEnemy = countBoard(piecesEnemy);

    Move bestMove = {"none", 0ULL, 0ULL};
    int bestEval = INT_MIN;

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);
    // std::shuffle(moves.begin(), moves.end(), std::mt19937(std::random_device{}()));

    for (const Move& move : moves) {
        auto piecesCopy = pieces; // engine
        auto piecesEnemyCopy = piecesEnemy; //player
        applyMoveMM(color, piecesCopy, piecesEnemyCopy, move);

        if (initialCheck(turn, color, piecesCopy, piecesEnemyCopy) != 0) continue; // illegal moves
        // if (initialCheck(-turn, -color, piecesEnemyCopy, piecesCopy)==2) bestEval = INT_MAX;

        int eval = minMax(depth - 1, depth, -turn, -color, piecesEnemyCopy, piecesCopy, -100000, 100000); //alpha and beta declar

        // if (initialCheck(-turn, -color, piecesEnemyCopy, piecesCopy) == 3)
        //     eval += 100; // if enemy is in checkmate, add bonus
        
        // if (initialCheck(-turn, -color, piecesEnemyCopy, piecesCopy) == 3){
        //     uint64_t king = getPieceBitboard(piecesEnemyCopy, "king");
        //     if(validMoves(king, -color, piecesEnemyCopy, piecesCopy).size() == 0) 
        //         eval+=50;
        // }

        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        } else if (eval == bestEval && rand() % 2 == 0) {
            bestEval = eval;
            bestMove = move;
        }
    }

    return bestMove;
}