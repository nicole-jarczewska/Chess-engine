// #include "engine_main.cpp"
#include "engine_support.cpp"

#include "moves.cpp"
#include "special.cpp"
#include "board.cpp"


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


void sendMoveToPython(const Move& move) {
    int fromIndex = __builtin_ctzll(move.from);
    int toIndex = __builtin_ctzll(move.to);
    std::cout << fromIndex << " " << toIndex << std::endl;
    std::cout.flush();
}

int gameLoop(){
    int gameState = 0; // 0: playing, 1: check, 2: you win, 3: enemy wins, 4: draw
    int turn = 1; // 1: your turn, -1: enemy turn
    int COLOR = 1; // your color // 1: white, -1: black
    int depth = 4;

    std::vector<std::pair<std::string, uint64_t*>> piecesPTR;
    std::vector<std::pair<std::string, uint64_t*>> piecesEnemyPTR;

    setGame(gameState, turn, COLOR, piecesPTR, piecesEnemyPTR);
    auto pieces = dereferencePieces(piecesPTR);
    auto piecesEnemy = dereferencePieces(piecesEnemyPTR);

    uint64_t board = (COLOR == 1) ? BOARD.boardWhite : BOARD.boardBlack;
    uint64_t boardEnemy = (COLOR == 1) ? BOARD.boardBlack : BOARD.boardWhite;
    for(auto& [_, bb] : pieces) board |= bb;
    for(auto& [_, bb] : piecesEnemy) boardEnemy |= bb;

    Move movePlayer;
    Move moveEngine;

    while(gameState == 0 || gameState == 1){
        if (turn == 1) {
            std::string input;
            if (!std::getline(std::cin, input)) return 4;
            if (input.find(' ') == std::string::npos) {
                int squareIndex = std::stoi(input);
                auto moves = validMoves((1ULL << squareIndex),board, boardEnemy, COLOR, pieces, piecesEnemy);
                std::cerr << "[DEBUG] Getting moves for piece index: " << squareIndex << std::endl;
                std::cerr << "[DEBUG] Bitboard: " << std::hex << (1ULL << squareIndex) << std::endl;
                for (const auto& move : moves) {
                    std::cout << __builtin_ctzll(move.to) << std::endl;
                }
                std::cout << std::endl; // terminator
                std::cout.flush();
                return 1; // continue loop
            }
            std::istringstream moveStream(input);
            std::string pieceType;
            int moveFrom, moveTo;

            if (!(moveStream >> pieceType >> moveFrom >> moveTo)) {
                std::cerr << "[ERROR] Invalid move input format. Expected: <piece> <from> <to>\n";
                continue;
            }

            movePlayer.pieceType = pieceType;
            movePlayer.from = 1ULL << moveFrom;
            movePlayer.to = 1ULL << moveTo;

            applyMove(COLOR, piecesPTR, piecesEnemyPTR, movePlayer);
        } else {
            Move moveEngine = findBestMove(turn, depth, -COLOR, piecesEnemy, pieces);
            applyMove(-COLOR, piecesEnemyPTR, piecesPTR, moveEngine);
            sendMoveToPython(moveEngine);
        }

        // Update turn, check state
        turn *= -1;

        auto piecesNow = dereferencePieces(piecesPTR);
        auto piecesEnemyNow = dereferencePieces(piecesEnemyPTR);

        board = 0; boardEnemy = 0;
        for (auto& [_, bb] : piecesNow) board |= bb;
        for (auto& [_, bb] : piecesEnemyNow) boardEnemy |= bb;

        if (turn == 1)
            gameState = initialCheck(turn, board, boardEnemy, COLOR, piecesNow, piecesEnemyNow);
        else
            gameState = initialCheck(turn, boardEnemy, board, -COLOR, piecesEnemyNow, piecesNow);

        if (getPieceBitboard(piecesNow, "king") == 0ULL) return 3;
        if (getPieceBitboard(piecesEnemyNow, "king") == 0ULL) return 2;
    }

    return 0;
}

int main() {
    // std::ios::sync_with_stdio(false);
    // std::cout << std::unitbuf; // auto flush after each output
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int gameOutCome = gameLoop();

    if(gameOutCome == 2) {
        std::cout << "You win!" << std::endl;
    } else if(gameOutCome == 3) {
        std::cout << "Enemy wins!" << std::endl;
    } else if(gameOutCome == 4) {
        std::cout << "It's a draw!" << std::endl;
    } else {
        std::cout << "Game continues." << std::endl;
    }

    return 0;
}