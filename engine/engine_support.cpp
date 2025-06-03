#include "include/engine.hpp"
// #include "moves.cpp"
// #include "special.cpp"
// #include "board.cpp"


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
        potentialEnemyMoves |= iterateMoves(bitboard, boardEnemy, board, -color, type);
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
        {"king", 1000000}
    };

    auto it = pieceValue.find(type);
    if (it != pieceValue.end()) {
        return it->second;
    }
    return 0; // Unknown piece
}

int evaluate(int color, std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy) { //int turn, uint64_t move, int color
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
    for (const auto& [type, bitboard] : pieces){
        score += popcountll(bitboard) * pieceValue(type);
        if(type == "pawn"){
        if (color == 1) score += popcountll(bitboard & 0x00FF000000000000ULL) * 5; // white pawns about to be promoted
        else score += popcountll(bitboard & 0x000000000000FF00ULL) * 5; // black pawns about to be promoted
        }
    }

    for (const auto& [type, bitboard] : piecesEnemy){
        score -= popcountll(bitboard) * pieceValue(type);
        if(type == "pawn"){
        if (color == 1) score -= popcountll(bitboard & 0x00FF000000000000ULL) * 5; // white pawns about to be promoted
        else score -= popcountll(bitboard & 0x000000000000FF00ULL) * 5; // black pawns about to be promoted
        }
    }

    uint64_t board = 0, boardEnemy = 0;
    for (auto& [_, bb] : pieces) board |= bb;
    for (auto& [_, bb] : piecesEnemy) boardEnemy |= bb;

    score += popcountll(board & 0x7E7E7E7E7E7E00ULL) * 2; // near the center
    score -= popcountll(boardEnemy & 0x7E7E7E7E7E7E00ULL) * 2; // enemy pieces near the center
    score += popcountll(board & 0x3C3C3C3C0000ULL) * 5; // control of the center 
    score -= popcountll(boardEnemy & 0x3C3C3C3C0000ULL) * 5; // enemy control of the center

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

void applyMoveMM(int color, std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy, const Move& move) {
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
            uint64_t onePiece = 1ULL << ctzll(bitboardCopy);
            // uint64_t onePiece = bitboardCopy & -bitboardCopy; // isolate LSB
            uint64_t moveMap = iterateMoves(onePiece, board, boardEnemy, color, type);
            
            while (moveMap) {
                uint64_t to = 1ULL << ctzll(moveMap);
                moves.push_back({type, onePiece, to});
                moveMap &= (moveMap - 1);
            }

            bitboardCopy &= (bitboardCopy - 1);
        }
    }
    return moves;
}

int minMax(int depth, int fullDepth, int turn, int color, uint64_t board, uint64_t boardEnemy,
std::vector<std::pair<std::string, uint64_t>> pieces, std::vector<std::pair<std::string, uint64_t>> piecesEnemy, int alpha, int beta) {

    if (depth == 0) {
        if (fullDepth % 2 == 0)
            return evaluate(color, pieces, piecesEnemy); // player turn
        else
            return evaluate(-color, piecesEnemy, pieces); // enemy turn
    }

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);
    int bestEval = (turn == 1) ? INT_MIN : INT_MAX;

    for (const Move& move : moves) {
            auto piecesCopy = pieces;
            auto piecesEnemyCopy = piecesEnemy;
            applyMoveMM(color, piecesCopy, piecesEnemyCopy, move);

            uint64_t boardCopy = 0, boardEnemyCopy = 0;
            for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
            for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

            // if(turn==-1 && initialCheck(turn, boardCopy, boardEnemyCopy, color, piecesCopy, piecesEnemyCopy)!=0) continue; // skip moves that result in check
            // else if(turn==1 && initialCheck(turn, boardEnemyCopy, boardCopy, -color, piecesEnemyCopy, piecesCopy)!=0) continue; // skip moves that result in check

            int eval = minMax(depth - 1, fullDepth, -turn, -color, boardEnemyCopy, boardCopy, piecesEnemyCopy, piecesCopy, alpha, beta);

            if(turn == 1){
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

    return bestEval;
}

// int minMax(int depth, int fullDepth, int turn, int color, uint64_t board, uint64_t boardEnemy,
//            std::vector<std::pair<std::string, uint64_t>> pieces,
//            std::vector<std::pair<std::string, uint64_t>> piecesEnemy,
//            int alpha, int beta) {

//     if (depth == 0) {
//         if (fullDepth % 2 == 0)
//             return evaluate(color, pieces, piecesEnemy); // player turn
//         else
//             return evaluate(-color, piecesEnemy, pieces); // enemy turn
//     }

//     auto moves = generateAllMoves(pieces, board, boardEnemy, color);
//     if (moves.empty()) {
//         // Handle stalemate or no-legal-move scenario
//         return evaluate(color, pieces, piecesEnemy); // or some neutral/loss score
//     }

//     if (turn == 1) { // Maximizing player
//         int maxEval = -99999;
//         for (const Move& move : moves) {
//             auto piecesCopy = pieces;
//             auto piecesEnemyCopy = piecesEnemy;
//             applyMoveMM(piecesCopy, piecesEnemyCopy, move);

//             uint64_t boardCopy = 0, boardEnemyCopy = 0;
//             for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
//             for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

//             int eval = minMax(depth - 1, fullDepth, -turn, -color,
//                               boardEnemyCopy, boardCopy,
//                               piecesEnemyCopy, piecesCopy,
//                               alpha, beta);

//             maxEval = std::max(maxEval, eval);
//             alpha = std::max(alpha, eval);
//             if (beta <= alpha)
//                 break; // Beta cutoff
//         }
//         return maxEval;
//     } else { // Minimizing player
//         int minEval = 99999;
//         for (const Move& move : moves) {
//             auto piecesCopy = pieces;
//             auto piecesEnemyCopy = piecesEnemy;
//             applyMoveMM(piecesCopy, piecesEnemyCopy, move);

//             uint64_t boardCopy = 0, boardEnemyCopy = 0;
//             for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
//             for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

//             int eval = minMax(depth - 1, fullDepth, -turn, -color,
//                               boardEnemyCopy, boardCopy,
//                               piecesEnemyCopy, piecesCopy,
//                               alpha, beta);

//             minEval = std::min(minEval, eval);
//             beta = std::min(beta, eval);
//             if (beta <= alpha)
//                 break; // Alpha cutoff
//         }
//         return minEval;
//     }
// }

Move findBestMove(int turn, int depth, int color, std::vector<std::pair<std::string, uint64_t>> pieces,
std::vector<std::pair<std::string, uint64_t>> piecesEnemy) {

    uint64_t board = 0, boardEnemy = 0;
    for (auto& [_, bb] : pieces) board |= bb;
    for (auto& [_, bb] : piecesEnemy) boardEnemy |= bb;

    Move bestMove;
    int bestEval = INT_MIN;

    auto moves = generateAllMoves(pieces, board, boardEnemy, color);
    std::shuffle(moves.begin(), moves.end(), std::mt19937(std::random_device{}()));

    for (const Move& move : moves) {
        auto piecesCopy = pieces;
        auto piecesEnemyCopy = piecesEnemy;
        applyMoveMM(color, piecesCopy, piecesEnemyCopy, move);

        uint64_t boardCopy = 0, boardEnemyCopy = 0;
        for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
        for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

        if (initialCheck(turn, boardCopy, boardEnemyCopy, color, piecesCopy, piecesEnemyCopy) != 0) continue; // illegal moves

        int eval = minMax(depth - 1, depth, -turn, -color, boardEnemyCopy, boardCopy, piecesEnemyCopy, piecesCopy, -100000, 100000); //alpha and beta declar

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

std::vector<Move> validMoves(uint64_t piece, uint64_t board, uint64_t boardEnemy, int color,
std::vector<std::pair<std::string, uint64_t>>& pieces, std::vector<std::pair<std::string, uint64_t>>& piecesEnemy) {
    std::vector<Move> moves;
    std::string pieceType;

    for (auto& [type, bitboard] : pieces) {
        if (bitboard & piece) {
            pieceType = type;
            break;
        }
    }

    auto bitboardCopy = piece;

    while (bitboardCopy) {
        uint64_t onePiece = 1ULL << ctzll(bitboardCopy);
        uint64_t moveMap = iterateMoves(onePiece, board, boardEnemy, color, pieceType);
            
        while (moveMap) {
                uint64_t to = 1ULL << ctzll(moveMap);

                auto piecesCopy = pieces;
                auto piecesEnemyCopy = piecesEnemy;

                Move candidateMove{pieceType, onePiece, to};
                applyMoveMM(color, piecesCopy, piecesEnemyCopy, candidateMove);

                uint64_t boardCopy = 0, boardEnemyCopy = 0;
                for (auto& [_, bb] : piecesCopy) boardCopy |= bb;
                for (auto& [_, bb] : piecesEnemyCopy) boardEnemyCopy |= bb;

                if (initialCheck(1, boardCopy, boardEnemyCopy, color, piecesCopy, piecesEnemyCopy) == 0)
                    moves.push_back(candidateMove);

                moveMap &= (moveMap - 1);
        }

        bitboardCopy &= (bitboardCopy - 1);
    }

    return moves;
}