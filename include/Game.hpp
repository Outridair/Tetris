//
// Created by David Rynearson on 26.05.25.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Board.hpp"
#include "Tetromino.hpp"
#include "Renderer.hpp"

enum class GameState {
    StartScreen,
    Playing,
    Paused,
    GameOver
};

class Game {
public:
    bool init(const char* title, int width, int height);
    void run();
    ~Game();
private:
    GameState state = GameState::StartScreen;

    SDL_Texture* startTexture = nullptr;
    SDL_Texture* startScreenTexture = nullptr;
    SDL_Rect startDst = {};
    SDL_Rect startScreenDst = {};
    Uint32 blinkTimer = 0;
    bool showText = true;

    SDL_Texture* gameOverTexture     = nullptr;
    SDL_Texture* gameOverInstrTexture= nullptr;
    SDL_Rect     gameOverDst;
    SDL_Rect     instrDst;

    SDL_Texture* pauseTexture = nullptr;
    SDL_Rect pauseDst;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;

    bool running = false;

    int score = 0;
    int linesCleared = 0;
    int level = 1;

    TTF_Font* font = nullptr;

    Board board;
    Tetromino* currentPiece = nullptr;
    Renderer* gfx = nullptr;

    Uint32 dropInterval = 500;
    Uint32 lastDropTime = 0;

    bool checkCollision(int newX, int newY) const;
    void lockPiece();
    void spawnNewPiece();

    void processInput();
    void update();
    void render();

    void startFreshGame();
};



#endif //GAME_HPP
