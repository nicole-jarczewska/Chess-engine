#include "include/chess.hpp"

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