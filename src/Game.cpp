//
// Created by David Rynearson on 26.05.25.
//

#include "../include/Game.hpp"
#include <iostream>

bool Game::init(const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }
    window = SDL_CreateWindow(title, 100, 100, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!window || !renderer) return false;
    currentPiece = new Tetromino(static_cast<Tetromino::Type>(rand() % 7));
    gfx = new Renderer(renderer);
    running = true;
    return true;
}

void Game::run() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        processInput();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        // TODO handle left/right/down/ rotate/ drop
    }
}

bool Game::checkCollision(int newX, int newY) const {
    const auto& shape = currentPiece->currentShape();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!shape[i][j]) continue;

            // Board::isOccupied checks both grid bounds and filled cells
            if (board.isOccupied(newX + j, newY + i))
                return true;
        }
    }
    return false;
}

void Game::lockPiece() {
    const auto& shape = currentPiece->currentShape();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (shape[i][j]) {
                board.occupy(currentPiece->x + j,
                             currentPiece->y + i);
            }
        }
    }
}

void Game::spawnNewPiece() {
    delete currentPiece;
    std::srand(unsigned(std::time(nullptr)));  // seed once in practice—moved here for clarity
    currentPiece = new Tetromino(
        static_cast<Tetromino::Type>(std::rand() % 7)
    );
    // reset drop timer so the new piece doesn't immediately fall
    lastDropTime = SDL_GetTicks();
}

void Game::update() {
    Uint32 now = SDL_GetTicks();

    if (now - lastDropTime >= dropInterval) {
        lastDropTime = now;

        int tryY = currentPiece->y + 1;
        // 1) If no collision, move down
        if (!checkCollision(currentPiece->x, tryY)) {
            currentPiece->y = tryY;
        }
        else {
            // 2) Otherwise lock it in place...
            lockPiece();

            // 3) ...and clear any full lines
            int linesCleared = board.clearFullLines();
            // (you can update score/level here based on linesCleared)

            // 4) spawn a fresh piece
            spawnNewPiece();

            // 5) check for game-over (new piece immediately collides)
            if (checkCollision(currentPiece->x, currentPiece->y)) {
                running = false;
                std::cout << "Game Over!" << std::endl;
            }
        }
    }
}


void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    gfx->drawBoard(board);
    gfx->drawTetromino(*currentPiece);

    SDL_RenderPresent(renderer);
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

