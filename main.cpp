// #include "engine_main.cpp"
#include "engine.cpp"
#include "include/chess.hpp"

// int gameLoop(){
//     int gameState = 0; // 0: playing, 1: check, 2: you win, 3: enemy wins, 4: draw
//     int turn = 1; // 1: your turn, -1: enemy turn
//     int COLOR = 1; // your color // 1: white, -1: black
//     int depth = 4;

//     std::vector<std::pair<std::string, uint64_t*>> piecesPTR;
//     std::vector<std::pair<std::string, uint64_t*>> piecesEnemyPTR;

//     setGame(gameState, turn, COLOR, piecesPTR, piecesEnemyPTR);
//     auto pieces = dereferencePieces(piecesPTR);
//     auto piecesEnemy = dereferencePieces(piecesEnemyPTR);

//     uint64_t board = (COLOR == 1) ? BOARD.boardWhite : BOARD.boardBlack;
//     uint64_t boardEnemy = (COLOR == 1) ? BOARD.boardBlack : BOARD.boardWhite;
//     for(auto& [_, bb] : pieces) board |= bb;
//     for(auto& [_, bb] : piecesEnemy) boardEnemy |= bb;

//     Move movePlayer;
//     Move moveEngine;

//     while(gameState == 0 || gameState == 1){
//         if (turn == 1) {       

//             applyMove(COLOR, piecesPTR, piecesEnemyPTR, movePlayer);
//         } else {
//             Move moveEngine = findBestMove(turn, depth, -COLOR, piecesEnemy, pieces);
//             applyMove(-COLOR, piecesEnemyPTR, piecesPTR, moveEngine);
//         }

//         // Update turn, check state
//         turn *= -1;

//         auto piecesNow = dereferencePieces(piecesPTR);
//         auto piecesEnemyNow = dereferencePieces(piecesEnemyPTR);

//         board = 0; boardEnemy = 0;
//         for (auto& [_, bb] : piecesNow) board |= bb;
//         for (auto& [_, bb] : piecesEnemyNow) boardEnemy |= bb;

//         if (turn == 1)
//             gameState = initialCheck(turn, COLOR, piecesNow, piecesEnemyNow);
//         else
//             gameState = initialCheck(turn, -COLOR, piecesEnemyNow, piecesNow);

//         if (getPieceBitboard(piecesNow, "king") == 0ULL) return 3;
//         if (getPieceBitboard(piecesEnemyNow, "king") == 0ULL) return 2;
//     }

//     return 0;
// }

int main() {
    BOARD.reset();
    BOARD.blackRook = BOARD.set_square(45);
    std::vector<std::pair<std::string, uint64_t>> me = dereferencePieces(piecesBlack);
    std::vector<std::pair<std::string, uint64_t>> ai = dereferencePieces(piecesWhite);

    BOARD.print_bitboard();
    int x =initialCheck(1, -1, me, ai);
    std::cout << "gamestate: " << x;
    return 0;
}