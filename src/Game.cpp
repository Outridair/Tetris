//
// Created by David Rynearson on 26.05.25.
//

#include "../include/Game.hpp"
#include <iostream>

bool Game::init(const char* title, int w, int h) {
    // 1) SDL / TTF / IMG
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

    int flags = MIX_INIT_OGG | MIX_INIT_MP3;

    if (!(Mix_Init(flags) & flags)) {
        std::cerr << "Mix_Init Error: " << Mix_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << "\n";
        return false;
    }

    menuBGM = Mix_LoadMUS("../assets/sound/music/mainMenuTheme.mp3");
    if (!menuBGM) std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << "\n";

    gameBGM = Mix_LoadMUS("../assets/sound/music/inGameMusic.mp3");
    if (!gameBGM) std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << "\n";

    sfxDrop = Mix_LoadWAV("../assets/sound/effects/blockDrop.ogg");
    sfxGameOver = Mix_LoadWAV("../assets/sound/effects/gameOver.ogg");
    sfxLevelUp = Mix_LoadWAV("../assets/sound/effects/levelUp.ogg");
    sfxLineClear = Mix_LoadWAV("../assets/sound/effects/lineClear.ogg");
    sfxMove = Mix_LoadWAV("../assets/sound/effects/move.ogg");
    sfxPause = Mix_LoadWAV("../assets/sound/effects/pause.ogg");
    sfxRotateCCW = Mix_LoadWAV("../assets/sound/effects/rotateCCW.ogg");
    sfxRotateCW = Mix_LoadWAV("../assets/sound/effects/rotateCW.ogg");
    sfxStartOrContinue = Mix_LoadWAV("../assets/sound/effects/startOrContinue.ogg");


    // 2) Load font
    // (or remove SDL_GetBasePath if you don't actually need it)
    font = TTF_OpenFont("../assets/Helvetica.ttc", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << "\n";
        return false;
    }

    // 3) Create window & renderer
    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error\n";
        return false;
    }

    // 4) Compute board offsets
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    int offsetX = (windowWidth  - BOARD_PIX_W) / 2;
    int offsetY = (windowHeight  - BOARD_PIX_H) / 2;

    // 5) “Press Any Key” texture
    SDL_Color white = {255,255,255,255};
    SDL_Surface* startTextSurf = TTF_RenderText_Blended(
        font, "Press Any Key to Start", white);
    if (!startTextSurf) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << "\n";
        return false;
    }
    startTexture = SDL_CreateTextureFromSurface(renderer, startTextSurf);
    int textW = startTextSurf->w,
        textH = startTextSurf->h;
    SDL_FreeSurface(startTextSurf);

    // 6) Logo texture
    startScreenTexture = IMG_LoadTexture(renderer,
        "../assets/Tetris_logo.png");
    if (!startScreenTexture) {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << "\n";
        return false;
    }
    int logoW, logoH;
    SDL_QueryTexture(startScreenTexture, nullptr, nullptr, &logoW, &logoH);

    // 7) Center cat+text group
    const int spacing = 20;
    int groupH = logoH + spacing + textH;
    int groupY = (windowHeight - groupH) / 2;
    startScreenDst = {
        (windowWidth - logoW) / 2,
        groupY,
        logoW,
        logoH
    };
    startDst = {
        (windowWidth - textW) / 2,
        groupY + logoH + spacing,
        textW,
        textH
    };

    // 8) “Game Over” textures
    SDL_Surface* goTitleSurf = TTF_RenderText_Blended(
        font, "Game Over", white);
    if (!goTitleSurf) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << "\n";
        return false;
    }
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, goTitleSurf);
    gameOverDst = {0,0, goTitleSurf->w, goTitleSurf->h};
    SDL_FreeSurface(goTitleSurf);

    SDL_Surface* goInstrSurf = TTF_RenderText_Blended(
        font, "Q: Quit    Enter: Continue?", white);
    if (!goInstrSurf) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << "\n";
        return false;
    }
    gameOverInstrTexture = SDL_CreateTextureFromSurface(renderer, goInstrSurf);
    instrDst = {0,0, goInstrSurf->w, goInstrSurf->h};
    SDL_FreeSurface(goInstrSurf);

    // center GameOver & instructions
    gameOverDst.x = (windowWidth - gameOverDst.w) / 2;
    gameOverDst.y = (windowHeight - gameOverDst.h) / 2 - 20;
    instrDst.x    = (windowWidth - instrDst.w)    / 2;
    instrDst.y    = gameOverDst.y + gameOverDst.h + 10;


    // Create Paused Texture.
    SDL_Surface* pauseSurf = TTF_RenderText_Blended(font, "Paused (Press P to unpause)", white);
    if (!pauseSurf) {
        std::cerr << "TTF_RenderText Error: " << TTF_GetError() << "\n";
        return false;
    }

    pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurf);
    pauseDst.w = pauseSurf->w;
    pauseDst.h = pauseSurf->h;
    SDL_FreeSurface(pauseSurf);

    pauseDst.x = (windowWidth  - pauseDst.w) / 2;
    pauseDst.y = (windowHeight  - pauseDst.h) / 2;

    // 9) Final setup
    gfx    = new Renderer(renderer, offsetX, offsetY, font);
    blinkTimer = SDL_GetTicks();
    showText   = true;

    // seed RNG & reset game state (spawns first piece properly)
    std::srand(unsigned(std::time(nullptr)));

    running = true;
    Mix_PlayMusic(menuBGM, -1);
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

void Game::startFreshGame() {
    board = Board();
    score        = 0;
    linesCleared = 0;
    level        = 1;

    delete currentPiece;
    currentPiece = nullptr;
    spawnNewPiece();
}

void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (e.type == SDL_KEYDOWN) {
            switch (state) {
                case GameState::StartScreen:
                    Mix_PlayChannel(-1, sfxStartOrContinue, 0);
                    startFreshGame();
                    Mix_PlayMusic(gameBGM, -1);
                    state = GameState::Playing;
                    lastDropTime = SDL_GetTicks();
                    break;

                case GameState::Playing:
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT:
                        case SDLK_a:
                                if (!checkCollision(currentPiece->x - 1, currentPiece->y)) currentPiece->x -= 1;
                                Mix_PlayChannel(-1, sfxMove, 0);
                                break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                                if (!checkCollision(currentPiece->x  + 1, currentPiece->y)) currentPiece->x += 1;
                                Mix_PlayChannel(-1, sfxMove, 0);
                                break;
                        case SDLK_DOWN:
                        case SDLK_s:
                                if (!checkCollision(currentPiece->x , currentPiece->y + 1)) currentPiece->y += 1;
                                Mix_PlayChannel(-1, sfxMove, 0);
                                break;
                        case SDLK_q:
                                currentPiece->tryRotateCCW(board);
                                Mix_PlayChannel(-1, sfxRotateCCW, 0);
                                break;
                        case SDLK_e:
                                currentPiece->tryRotateCW(board);
                                Mix_PlayChannel(-1, sfxRotateCW, 0);
                                break;
                        case SDLK_p:
                                Mix_PlayChannel(-1, sfxPause, 0);
                                Mix_HaltMusic();
                                state = GameState::Paused;
                            default: ;
                    }
                    break;
                case GameState::Paused:
                    if (e.key.keysym.sym == SDLK_p) {
                        Mix_PlayChannel(-1, sfxPause, 0);
                        Mix_PlayMusic(gameBGM, -1);
                        state = GameState::Playing;
                    }
                    break;
                case GameState::GameOver:
                    if (e.key.keysym.sym == SDLK_q) {
                        running = false;
                    }
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        startFreshGame();
                        Mix_PlayChannel(-1, sfxStartOrContinue, 0);
                        state = GameState::Playing;
                    }
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
    if (state != GameState::Playing) {
        // only the start‐screen needs blinking
        if (state == GameState::StartScreen) {
            if (now - blinkTimer > 500) {
                blinkTimer = now;
                showText   = !showText;
            }
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
                Mix_PlayChannel(-1, sfxLineClear, 0);
                linesCleared += cleared;
                static const int pointsPer[5] = {0, 100, 300, 500, 800 };
                score += pointsPer[cleared] * level;

                level = 1 + linesCleared / 10;

                dropInterval = std::max(50u, 500u - (level -1 ) * 25);
            }

            spawnNewPiece();

            if (checkCollision(currentPiece->x, currentPiece->y)) {
                state = GameState::GameOver;
                Mix_PlayChannel(-1, sfxGameOver, 0);
            }
        }
    }
}


void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect full{ 0, 0, windowWidth, windowHeight };

    switch (state) {
        case GameState::StartScreen:
            SDL_RenderCopy(renderer, startScreenTexture, nullptr, &startScreenDst);
            if (showText) {
                SDL_RenderCopy(renderer, startTexture, nullptr, &startDst);
            }
            break;

        case GameState::Playing:
            gfx->drawBoard(board);
            gfx->drawTetromino(*currentPiece);
            gfx->drawScore(score, level, linesCleared);
            break;

        case GameState::Paused:
            gfx->drawBoard(board);
            gfx->drawTetromino(*currentPiece);
            gfx->drawScore(score, level, linesCleared);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_RenderFillRect(renderer, &full);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseDst);
            break;



        case GameState::GameOver:
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverDst);
            SDL_RenderCopy(renderer, gameOverInstrTexture, nullptr, &instrDst);
            break;
    }

    SDL_RenderPresent(renderer);
}

Game::~Game() {
    if (startScreenTexture)   SDL_DestroyTexture(startScreenTexture);
    if (startTexture)         SDL_DestroyTexture(startTexture);
    if (gameOverTexture)      SDL_DestroyTexture(gameOverTexture);
    if (gameOverInstrTexture) SDL_DestroyTexture(gameOverInstrTexture);
    if (pauseTexture)         SDL_DestroyTexture(pauseTexture);

    if (menuBGM)                  Mix_FreeMusic(menuBGM);
    if (sfxRotateCCW)         Mix_FreeChunk(sfxRotateCCW);
    if (sfxDrop)              Mix_FreeChunk(sfxDrop);

    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    if (font) TTF_CloseFont(font);
    TTF_Quit();

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();

    delete currentPiece;
    delete gfx;
}

