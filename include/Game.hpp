//
// Created by David Rynearson on 26.05.25.
//

#ifndef GAME_HPP
#define GAME_HPP
#include <SDL.h>
#include "Board.hpp"
#include "Tetromino.hpp"
#include "Renderer.hpp"


class Game {
public:
    bool init(const char* title, int width, int height);
    void run();
    ~Game();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

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
};



#endif //GAME_HPP
