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
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!window || !renderer) return false;
    currentPiece = new Tetromino(static_cast<Tetromino::Type>(rand() % 7));
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    int offsetX = (winW  - BOARD_PIX_W) / 2;
    int offsetY = (winH  - BOARD_PIX_H) / 2;

    gfx = new Renderer(renderer, offsetX, offsetY);
    running = true;
    return true;
}

void Game::run() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
    }
}

void Game::processInput(int& pieceX, int& pieceY) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN) {
            std::cout << "Key pressed: " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;

            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    if (!checkCollision(pieceX - 1, pieceY)) pieceX--;
                    break;
                case SDLK_RIGHT:
                    if (!checkCollision(pieceX + 1, pieceY)) pieceX++;
                    break;
                case SDLK_DOWN:
                    if (!checkCollision(pieceX, pieceY)) pieceY++;
                    break;
            }
        }
    }
}

bool Game::checkCollision(int newX, int newY) const {
    const auto& shape = currentPiece->currentShape();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!shape[i][j]) continue;

            // Board::isOccupied checks both grid bounds and filled cells
            if (board.isOccupied(newX + j, newY + i)) {
                return true;
            }

        }
    }
    return false;
}

void Game::lockPiece() {
    const auto& shape = currentPiece->currentShape();
    auto type = currentPiece->getType();  // add a getter in Tetromino
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            if(shape[i][j])
                board.occupy(currentPiece->x + j,
                             currentPiece->y + i,
                             type);
}

void Game::spawnNewPiece() {
    delete currentPiece;
    currentPiece = new Tetromino(
        static_cast<Tetromino::Type>(std::rand() % 7)
    );
    currentPiece->x = BOARD_WIDTH/2 - 2;
    currentPiece->y = 0;
    // reset drop timer so the new piece doesn't immediately fall
    lastDropTime = SDL_GetTicks();
    SDL_Log("Spawned Tetromino: %s", toString(currentPiece->getType()));
}

void Game::update() {
    Uint32 now = SDL_GetTicks();
    processInput(currentPiece->x, currentPiece->y);

    if (now - lastDropTime >= dropInterval) {
        lastDropTime = now;

        int tryY = currentPiece->y + 1;
        if (!checkCollision(currentPiece->x, tryY)) {
            currentPiece->y = tryY;
        } else {
            // 2) Otherwise lock it in place...
            lockPiece();

            // 3) ...and clear any full lines
            int linesCleared = board.clearFullLines();
            // (you can update score/level here based on linesCleared)

            // 4) spawn a fresh piece
            spawnNewPiece();

            // // 5) check for game-over (new piece immediately collides)
            // if (checkCollision(currentPiece->x, currentPiece->y)) {
            //     running = false;
            //     std::cout << "Game Over!" << std::endl;
            // }
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

