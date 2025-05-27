

#include "engine.cpp"

int main() {
    int gameState = 0; // 0:palying, 1:check(either side), 2:victory, 3:enemy victory, 4:draw
    int turn = 1; // 1:you, -1:enemy
    int COLOR = 1;
    std::vector<std::pair<std::string, uint64_t>> pieces;
    std::vector<std::pair<std::string, uint64_t>> piecesEnemy;

    if (COLOR == 1) {
        pieces = piecesWhite;
        piecesEnemy = piecesBlack;
    } else {
        pieces = piecesBlack;
        piecesEnemy = piecesWhite;
    }


    // std::cout << "Piece at square 12: " << board.piece_at_square(1) << std::endl;

    // board.whiteKnight = moves.knight(board.whiteKnight);
    // std::cout << board.whiteKnight << std::endl; 

    return 0;
}