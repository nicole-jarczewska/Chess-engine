#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>

#include "include/chess.hpp"

#include "engine.cpp"
#include "gui_helper.cpp"

int handlePlayerMove(int squareClicked) {
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

            auto piecesEnemy = dereferencePieces(piecesEnemyPTR);

            auto legalMoves = validMoves(1, (1ULL << fromSquare), COLOR, pieces, piecesEnemy);
            if(legalMoves.empty())
                return 1;
            
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

            applyMove(COLOR, COLOR == 1 ? piecesWhite : piecesBlack, COLOR == 1 ? piecesBlack : piecesWhite, move);
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
    return 0;
}

int chooseColor(SDL_Renderer* renderer) {
    const int popupWidth = 300;
    const int popupHeight = 150;
    SDL_Window* popup = SDL_CreateWindow("Choose Color",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         popupWidth, popupHeight, 0);
    SDL_Renderer* popupRenderer = SDL_CreateRenderer(popup, -1, SDL_RENDERER_ACCELERATED);

    bool choosing = true;
    int chosenColor = 1;

    SDL_Event e;

    SDL_Rect whiteBtn = {50, 50, 80, 40};
    SDL_Rect blackBtn = {170, 50, 80, 40};

    while (choosing) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choosing = false;
                chosenColor = 1; // default to white if quit
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= whiteBtn.x && x <= whiteBtn.x + whiteBtn.w &&
                    y >= whiteBtn.y && y <= whiteBtn.y + whiteBtn.h) {
                    chosenColor = 1;
                    choosing = false;
                } else if (x >= blackBtn.x && x <= blackBtn.x + blackBtn.w &&
                           y >= blackBtn.y && y <= blackBtn.y + blackBtn.h) {
                    chosenColor = -1;
                    choosing = false;
                }
            }
        }

        SDL_SetRenderDrawColor(popupRenderer, 30, 30, 30, 255);
        SDL_RenderClear(popupRenderer);

        SDL_SetRenderDrawColor(popupRenderer, 200, 200, 200, 255);
        SDL_RenderFillRect(popupRenderer, &whiteBtn);
        SDL_RenderFillRect(popupRenderer, &blackBtn);

        SDL_SetRenderDrawColor(popupRenderer, 150, 150, 150, 255);
        SDL_Rect whiteLabel = {whiteBtn.x + 10, whiteBtn.y + 10, whiteBtn.w - 20, whiteBtn.h - 20};
        SDL_RenderFillRect(popupRenderer, &whiteLabel);

        SDL_SetRenderDrawColor(popupRenderer, 50, 50, 50, 255);
        SDL_Rect blackLabel = {blackBtn.x + 10, blackBtn.y + 10, blackBtn.w - 20, blackBtn.h - 20};
        SDL_RenderFillRect(popupRenderer, &blackLabel);

        SDL_RenderPresent(popupRenderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(popupRenderer);
    SDL_DestroyWindow(popup);

    isFlipped = (COLOR == -1);

    return chosenColor;
}

int chooseDifficulty(SDL_Renderer* renderer) {
    const int popupWidth = 400;
    const int popupHeight = 200;
    SDL_Window* popup = SDL_CreateWindow("Choose Difficulty",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         popupWidth, popupHeight, 0);
    SDL_Renderer* popupRenderer = SDL_CreateRenderer(popup, -1, SDL_RENDERER_ACCELERATED);

    // Load button textures
    SDL_Texture* easyBtnTex = nullptr;
    SDL_Texture* mediumBtnTex = nullptr;
    SDL_Texture* hardBtnTex = nullptr;

    auto loadTexture = [&](const std::string& path) -> SDL_Texture* {
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (!surf) {
            return nullptr;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(popupRenderer, surf);
        SDL_FreeSurface(surf);
        return tex;
    };

    easyBtnTex = loadTexture("assets/easy.png");
    mediumBtnTex = loadTexture("assets/medium.png");
    hardBtnTex = loadTexture("assets/hard.png");

    if (!easyBtnTex || !mediumBtnTex || !hardBtnTex) {
        SDL_DestroyRenderer(popupRenderer);
        SDL_DestroyWindow(popup);
        return 4; // default to medium if loading fails
    }

    SDL_Rect easyBtn = { 40, 70, 100, 50 };
    SDL_Rect mediumBtn = { 150, 70, 100, 50 };
    SDL_Rect hardBtn = { 260, 70, 100, 50 };

    bool choosing = true;
    int difficulty = 4; // default depth level (medium)

    SDL_Event e;

    while (choosing) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                choosing = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                if (x >= easyBtn.x && x <= easyBtn.x + easyBtn.w &&
                    y >= easyBtn.y && y <= easyBtn.y + easyBtn.h) {
                    difficulty = 2;
                    choosing = false;
                } else if (x >= mediumBtn.x && x <= mediumBtn.x + mediumBtn.w &&
                           y >= mediumBtn.y && y <= mediumBtn.y + mediumBtn.h) {
                    difficulty = 4;
                    choosing = false;
                } else if (x >= hardBtn.x && x <= hardBtn.x + hardBtn.w &&
                           y >= hardBtn.y && y <= hardBtn.y + hardBtn.h) {
                    difficulty = 5;
                    choosing = false;
                }
            }
        }

        SDL_SetRenderDrawColor(popupRenderer, 30, 30, 30, 255);
        SDL_RenderClear(popupRenderer);

        SDL_RenderCopy(popupRenderer, easyBtnTex, nullptr, &easyBtn);
        SDL_RenderCopy(popupRenderer, mediumBtnTex, nullptr, &mediumBtn);
        SDL_RenderCopy(popupRenderer, hardBtnTex, nullptr, &hardBtn);

        SDL_RenderPresent(popupRenderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(easyBtnTex);
    SDL_DestroyTexture(mediumBtnTex);
    SDL_DestroyTexture(hardBtnTex);
    SDL_DestroyRenderer(popupRenderer);
    SDL_DestroyWindow(popup);

    return difficulty;
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

    window = SDL_CreateWindow("Chess<3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
                // int gameover = handlePlayerMove(square);
                if(handlePlayerMove(square) == 1){
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

        SDL_Delay(10);
    }

    if (gameState == 2) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You won!!!!", window);
    } else if (gameState == 3) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You lost :((", window);
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
