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
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << "\n";
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << "\n";
        return false;
    }

    char* base = SDL_GetBasePath();
    std::string fontPath = "../assets/Helvetica.ttc";
    SDL_free(base);
    font = TTF_OpenFont(fontPath.c_str(), 24);    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
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

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "Press Any Key to Start", white);
    startTexture = SDL_CreateTextureFromSurface(renderer, surf);
    startDst.w = surf->w;
    startDst.h = surf->h;
    SDL_FreeSurface(surf);

    startScreenTexture = IMG_LoadTexture(renderer, "../assets/Tetris_logo.png");
    SDL_QueryTexture(startScreenTexture, nullptr, nullptr, &w, &h);
    startScreenDst.w = w;
    startScreenDst.h = h;

    SDL_GetWindowSize(window, &w, &h);
    int spacing = 20;

    int groupH = startScreenDst.h + spacing + startDst.h;
    int groupY = (h - groupH) / 2;

    startScreenDst.x = (w - startScreenDst.w) / 2;
    startScreenDst.y = groupY;

    startDst.x = (w - startDst.w) / 2;
    startDst.y = startScreenDst.y + startScreenDst.h + spacing;

    blinkTimer = SDL_GetTicks();
    showText = true;

    score        = 0;
    linesCleared = 0;
    level        = 1;

    gfx = new Renderer(renderer, offsetX, offsetY, font);
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
        if (e.type == SDL_KEYDOWN) {
            if (state == GameState::StartScreen) {
                state = GameState::Playing;
                spawnNewPiece();
                lastDropTime = SDL_GetTicks();
            } else {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                        if (!checkCollision(currentPiece->x - 1, currentPiece->y)) currentPiece->x -= 1;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (!checkCollision(currentPiece->x  + 1, currentPiece->y)) currentPiece->x += 1;
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (!checkCollision(currentPiece->x , currentPiece->y + 1)) currentPiece->y += 1;
                        break;
                    case SDLK_q:
                        currentPiece->tryRotateCCW(board);
                        break;
                    case SDLK_e:
                        currentPiece->tryRotateCW(board);
                        break;
                }
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
    SDL_Log("Locked Tetromino: %s", toString(type));
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
    if (state == GameState::StartScreen) {
        if (now - blinkTimer > 500) {
            blinkTimer = now;
            showText = !showText;
        }
        return;
    }
    if (now - lastDropTime >= dropInterval) {
        lastDropTime = now;

        int tryY = currentPiece->y + 1;
        if (!checkCollision(currentPiece->x, tryY)) {
            currentPiece->y = tryY;
        } else {
            lockPiece();

            int cleared = board.clearFullLines();
            if (cleared > 0) {
                linesCleared += cleared;
                static const int pointsPer[5] = {0, 100, 300, 500, 800 };
                score += pointsPer[cleared] * level;

                level = 1 + linesCleared / 10;

                dropInterval = std::max(50u, 500u - (level -1 ) * 25);
            }

            spawnNewPiece();

            if (checkCollision(currentPiece->x, currentPiece->y)) {
                running = false;
                std::cout << "Game Over!" << std::endl;
            }
        }
    }
}


void Game::render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (state == GameState::StartScreen) {
        SDL_RenderCopy(renderer, startScreenTexture, nullptr, &startScreenDst);
        if (showText) {
            SDL_RenderCopy(renderer, startTexture, nullptr, &startDst);
        }
    } else {
        gfx->drawBoard(board);
        gfx->drawTetromino(*currentPiece);
        gfx->drawScore(score, level, linesCleared);
    }

    SDL_RenderPresent(renderer);
}

Game::~Game() {
    if (font) TTF_CloseFont(font);
    if (startScreenTexture) SDL_DestroyTexture(startScreenTexture);
    if (startTexture) SDL_DestroyTexture(startTexture);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

