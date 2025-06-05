#include "include/chess.hpp"

void resetGame() {
    turn = (COLOR == 1 ? 1 : -1);
    gameState = 0;
    selectedSquare = -1;
    highlightedSquares.clear();
    BOARD.reset();
    BOARD.flip(isFlipped);
}

std::string getPieceTypeAtSquare(int square, int color) {
    for (const auto& [type, bbPtr] : piecesPTR) {
        if ((*bbPtr) & (1ULL << square)) {
            return type;
        }
    }
    return "pawn"; // fallback
}

bool loadTextures() {
    const std::vector<std::string> pieceNames = {
        "wpawn", "wknight", "wbishop", "wrook", "wqueen", "wking",
        "bpawn", "bknight", "bbishop", "brook", "bqueen", "bking"
    };

    for (const auto& name : pieceNames) {
        std::string path = "assets/" + name + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) return false;

        pieceTextures[name] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    return true;
}

void drawBoard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw tiles
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            SDL_Rect tile = { col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            bool isLight = (row + col) % 2 == 0;
            SDL_SetRenderDrawColor(renderer,
                isLight ? 240 : 181,
                isLight ? 217 : 136,
                isLight ? 181 : 99,
                255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    for (int sq : highlightedSquares) { // highlighted tiles to show valid moves
        int row = sq / 8;
        int col = sq % 8;
        int guiRow = isFlipped ? row : (7 - row);

        if (((row + col) % 2 == 1 && COLOR == 1)||((row + col) % 2 == 0 && COLOR == -1)) { // light tiles for white player
            SDL_SetRenderDrawColor(renderer, 60, 200, 90, 25); // lighter color for light tales
        } else {
            SDL_SetRenderDrawColor(renderer, 20, 150, 50, 15); // darker color for dark tiles
        }

        SDL_Rect dst = { col * TILE_SIZE, guiRow * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw pieces
    auto drawPiece = [&](const std::string& id, uint64_t bitboard) {
        for (int i = 0; i < 64; ++i) {
            if (bitboard & (1ULL << i)) {
                isFlipped = (COLOR == -1);
                int row = i / 8;
                int col = i % 8;
                int guiRow = isFlipped ? row : (7 - row);
                SDL_Rect dst = { col * TILE_SIZE, guiRow * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, pieceTextures[id], nullptr, &dst);
            }
        }
    };

    drawPiece("wpawn", BOARD.whitePawn);
    drawPiece("wknight", BOARD.whiteKnight);
    drawPiece("wbishop", BOARD.whiteBishop);
    drawPiece("wrook", BOARD.whiteRook);
    drawPiece("wqueen", BOARD.whiteQueen);
    drawPiece("wking", BOARD.whiteKing);
    drawPiece("bpawn", BOARD.blackPawn);
    drawPiece("bknight", BOARD.blackKnight);
    drawPiece("bbishop", BOARD.blackBishop);
    drawPiece("brook", BOARD.blackRook);
    drawPiece("bqueen", BOARD.blackQueen);
    drawPiece("bking", BOARD.blackKing);

    SDL_RenderPresent(renderer);
}