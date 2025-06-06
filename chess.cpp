#include "include/chess.hpp"

#include "engine.cpp"
#include "gui_helper.cpp"
#include "popups.cpp"

void playerMove(int squareClicked) {
    static int fromSquare = -1;

    if (fromSquare == -1) {
        auto pieces = dereferencePieces(piecesPTR);
        uint64_t mask = 1ULL << squareClicked;

        bool isOwnPiece = false;
        for (const auto& [_, bb] : pieces) {
            if (bb & mask) {
                isOwnPiece = true;
                break;
            }
        }

        if (isOwnPiece) {
            fromSquare = squareClicked;
            selectedSquare = squareClicked;
            std::string pieceType = getPieceTypeAtSquare(fromSquare, COLOR);

            auto piecesEnemy = dereferencePieces(piecesEnemyPTR);

            auto legalMoves = validMoves((1ULL << fromSquare), COLOR, pieces, piecesEnemy);

            highlightedSquares.clear();
            for (auto& move : legalMoves) {
                int toSquare = __builtin_ctzll(move.to);
                highlightedSquares.push_back(toSquare);
            }
        }
    } 
    else {
        if (std::find(highlightedSquares.begin(), highlightedSquares.end(), squareClicked) != highlightedSquares.end()) {
            int toSquare = squareClicked;
            std::string pieceType = getPieceTypeAtSquare(fromSquare, COLOR);

            Move move;
            move.pieceType = pieceType;
            move.from = 1ULL << fromSquare;
            move.to = 1ULL << toSquare;

            applyMove(COLOR, piecesPTR, piecesEnemyPTR, move);
            turn = -turn;
            
            fromSquare = -1;
            selectedSquare = -1;
            highlightedSquares.clear();
        } else {
            // if not clicked on a valid move, reset, let player check another piece
            fromSquare = -1;
            selectedSquare = -1;
            highlightedSquares.clear();
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) == 0) {
        std::cerr << "cannot open.\n";
        return 1;
    }

    SDL_Window* tempWindow = SDL_CreateWindow("temp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, SDL_WINDOW_HIDDEN);
    SDL_Renderer* tempRenderer = SDL_CreateRenderer(tempWindow, -1, SDL_RENDERER_ACCELERATED);

    COLOR = chooseColor(tempRenderer); // let player choose color
    int depth = chooseDifficulty(tempRenderer); // let player choose difficulty

    turn = (COLOR == 1) ? 1 : -1; //set turn based on color
    isFlipped = (COLOR == -1); // flip board if player is black

    piecesPTR = COLOR == 1 ? piecesWhite : piecesBlack;
    piecesEnemyPTR = COLOR == 1 ? piecesBlack : piecesWhite;

    SDL_DestroyRenderer(tempRenderer);
    SDL_DestroyWindow(tempWindow);

    window = SDL_CreateWindow("~Chess~", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!loadTextures()) return 1;

    BOARD.reset();
    BOARD.flip(isFlipped);

    bool quit = false;
    SDL_Event event;

    while (!quit && ( gameState == 0 || gameState == 1)) {
        drawBoard();

        // setGame(gameState, turn, COLOR, piecesPTR, piecesEnemyPTR);

        if (turn == -1) { //enemy turn
            auto pieces = dereferencePieces(piecesPTR);
            auto piecesEnemy = dereferencePieces(piecesEnemyPTR);
            Move move = findBestMove(turn, depth, -COLOR, piecesEnemy, pieces);
            if(move.pieceType == "none" && move.to == 0ULL && move.from == 0ULL) {
                if(initialCheck(-1, -COLOR, piecesEnemy, pieces) == 0){
                    gameState = 4; // draw
                    quit = true;
                    break;
                }
                else{
                    gameState = 2; // player wins
                    quit = true;
                    break;
                } 
            }
            applyMove(-COLOR, piecesEnemyPTR, piecesPTR, move);
            turn = -turn;
        }

        auto pieces = dereferencePieces(piecesPTR);
        auto piecesEnemy = dereferencePieces(piecesEnemyPTR);

        // gameState = initialCheck(turn, COLOR, pieces, piecesEnemy); // check if palyer is being checked

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;

            else if (event.type == SDL_MOUSEBUTTONDOWN && turn == 1) {
                int x = event.button.x / TILE_SIZE;
                int y = isFlipped ? (event.button.y / TILE_SIZE) : (7 - (event.button.y / TILE_SIZE));
                int square = y * 8 + x;

                // gameState = initialCheck(turn, COLOR, pieces, piecesEnemy); // check if player is being checked
                // if( gameState == 2 || gameState == 3) {
                //     quit = true;
                //     break;
                // }

                bool noMoves = true;

                for (auto& [type, bitboard] : pieces) { // cheching if the player has any legal moves
                    if (bitboard == 0) continue; // skip if no pieces of this type
                        auto movesToCheck = validMoves(bitboard, COLOR, pieces, piecesEnemy);
                        if(movesToCheck.size()!=0){
                            noMoves = false;
                            break;
                        } 
                }

                if(noMoves == true) {
                    if(initialCheck(1, COLOR, pieces, piecesEnemy) == 0){
                        gameState = 4; // draw
                        quit = true;
                        break;
                    }
                    else{
                        gameState = 3; // player wins
                        quit = true;
                        break;
                    } 
                }

                playerMove(square);
            }
                
            else if (event.type == SDL_KEYDOWN) {
                const Uint8* keystate = SDL_GetKeyboardState(nullptr);
                    if ((keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) && event.key.keysym.sym == SDLK_r) {
                        resetGame();
                    }
            }
        }

        pieces = dereferencePieces(piecesPTR);
        piecesEnemy = dereferencePieces(piecesEnemyPTR);

        // gameState = initialCheck(turn, -COLOR, piecesEnemy, pieces); // check if enemy is being checked

        if (getPieceBitboard(pieces, "king") == 0ULL) gameState = 3;
        if (getPieceBitboard(piecesEnemy, "king") == 0ULL)  gameState = 2;
        
        turnsTaken++;

        SDL_Delay(10);
    }

    if (gameState == 2) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You won!!!!", window);
    } else if (gameState == 3) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost :(", window);
    } else if (gameState == 4) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Draw.", window);
    }

    for (auto& [_, tex] : pieceTextures) SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
