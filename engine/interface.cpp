// // engine_interface.cpp
// #include "engine_support.cpp"

// extern "C" {

// static int COLOR = 1;
// static int turn = 1;
// static int gameState = 0;
// static int depth = 4;

// static std::vector<std::pair<std::string, uint64_t*>> piecesPTR;
// static std::vector<std::pair<std::string, uint64_t*>> piecesEnemyPTR;
// static std::vector<std::pair<std::string, uint64_t>> pieces;
// static std::vector<std::pair<std::string, uint64_t>> piecesEnemy;

// void init_game(int color) {
//     COLOR = color;
//     turn = 1;
//     gameState = 0;
//     setGame(gameState, turn, COLOR, piecesPTR, piecesEnemyPTR);
//     pieces = dereferencePieces(piecesPTR);
//     piecesEnemy = dereferencePieces(piecesEnemyPTR);
// }

// uint64_t get_combined_board() {
//     uint64_t b = 0;
//     for (auto& [_, bb] : pieces) b |= bb;
//     return b;
// }

// uint64_t get_piece_bb(const char* name) {
//     for (auto& [n, bb] : pieces)
//         if (n == name) return bb;
//     return 0;
// }

// bool make_player_move(int from, int to) {
//     Move move;
//     move.from = from;
//     move.to = to;
//     // You should ideally validate here
//     applyMove(COLOR, piecesPTR, piecesEnemyPTR, move);
//     pieces = dereferencePieces(piecesPTR);
//     piecesEnemy = dereferencePieces(piecesEnemyPTR);
//     turn *= -1;
//     return true;
// }

// Move make_ai_move() {
//     Move move = findBestMove(turn, depth, -COLOR, piecesEnemy, pieces);
//     applyMove(-COLOR, piecesEnemyPTR, piecesPTR, move);
//     pieces = dereferencePieces(piecesPTR);
//     piecesEnemy = dereferencePieces(piecesEnemyPTR);
//     turn *= -1;
//     return move;
// }

// int get_game_state() {
//     uint64_t board = 0, boardEnemy = 0;
//     for (auto& [_, bb] : pieces) board |= bb;
//     for (auto& [_, bb] : piecesEnemy) boardEnemy |= bb;
//     if (turn == 1) return initialCheck(turn, board, boardEnemy, COLOR, pieces, piecesEnemy);
//     else return initialCheck(turn, boardEnemy, board, -COLOR, piecesEnemy, pieces);
// }
// }
