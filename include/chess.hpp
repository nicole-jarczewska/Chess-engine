#pragma once
#include "libraries.hpp"

const int TILE_SIZE = 80;
const int BOARD_SIZE = 8;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
std::unordered_map<std::string, SDL_Texture*> pieceTextures;

// can be changed
int gameState = 0; // 0 = playing, 1 = check, 2 = you win, 3 = enemy wins, 4 = draw
int depth = 4; // default medium dif 

// helper variables
int turn = 1; // 1 = player, -1 = engine
int COLOR = 1; // color of the player, 1 = white, -1 = black
bool isFlipped = false; // true if black pieces are at the bottom
int turnsTaken = 0; // number of turns that have taken place

std::vector<std::pair<std::string, uint64_t*>> piecesPTR;
std::vector<std::pair<std::string, uint64_t*>> piecesEnemyPTR;

int selectedSquare = -1;
std::vector<int> highlightedSquares;