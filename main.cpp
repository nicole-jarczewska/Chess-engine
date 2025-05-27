// #include "engine_main.cpp"
#include "engine_support.cpp"

int main() {
    int gameState = 0; // 0: playing, 1: check, 2: you win, 3: enemy wins, 4: draw
    int turn = 1; // 1: your turn, -1: enemy turn
    int COLOR = 1; // your color // 1: white, -1: black

    std::vector<std::pair<std::string, uint64_t>> pieces;
    std::vector<std::pair<std::string, uint64_t>> piecesEnemy;

    setGame(gameState, turn, COLOR, pieces, piecesEnemy);
    BOARD.print_bitboard();
    applyMove(pieces, piecesEnemy, {"king", 0x4000000000000000ULL, 0x8000000000000000ULL}); // Example move: move white king from h1 to g1
    BOARD.print_bitboard();

    // std::cout << "Game started. Your turn." << std::endl;
    // BOARD.print_bitboard();

    // for (auto& [_, bb] : piecesWhite) BOARD.boardWhite |= bb;
    // for (auto& [_, bb] : piecesBlack) BOARD.boardBlack |= bb;

    // uint64_t board = (COLOR == 1) ? BOARD.boardWhite : BOARD.boardBlack;
    // uint64_t boardEnemy = (COLOR == 1) ? BOARD.boardBlack : BOARD.boardWhite;

    // if(turn == 1){
    //     gameState = initialCheck(turn, board, boardEnemy, COLOR, pieces, piecesEnemy);
    //     std::cout << "state: " << gameState << std::endl;
    //     Move move = findBestMove(turn, 2, COLOR, pieces, piecesEnemy);
    //     applyMove(pieces, piecesEnemy, move);
    // }
    // else if(turn == -1) {
    //     gameState = initialCheck(turn, boardEnemy, board, COLOR, piecesEnemy, pieces);
    //     std::cout << "state: " << gameState << std::endl;
    //     Move move = findBestMove(turn, 2, COLOR, piecesEnemy, pieces);
    //     applyMove(piecesEnemy, pieces, move);
    // }

    // // Only make move if game isn't already over
    // if (gameState == 0 || gameState == 1) {

    //     for (auto& [_, bb] : piecesWhite) BOARD.boardWhite |= bb;
    //     for (auto& [_, bb] : piecesBlack) BOARD.boardBlack |= bb;

    //     BOARD.print_bitboard();

    //     // Re-check state after move
    //     board = (COLOR == 1) ? BOARD.boardWhite : BOARD.boardBlack;
    //     boardEnemy = (COLOR == 1) ? BOARD.boardBlack : BOARD.boardWhite;
    // }

    return 0;
}