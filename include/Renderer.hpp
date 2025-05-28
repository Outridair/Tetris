//
// Created by David Rynearson on 27.05.25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <SDL.h>
#include "Board.hpp"
#include "Tetromino.hpp"

constexpr int CELL_SIZE = 30;                   // size of one block in pixels
constexpr int BOARD_PIX_W = BOARD_WIDTH * CELL_SIZE;
constexpr int BOARD_PIX_H = BOARD_HEIGHT * CELL_SIZE;

class Renderer {
public:
    Renderer(SDL_Renderer* ren, int offsetX, int offsetY) : ren(ren), offsetX(offsetX), offsetY(offsetY) {}
    void drawBoard(const Board& board);
    void drawTetromino(const Tetromino& t);

private:
    SDL_Renderer* ren;
    int offsetX;
    int offsetY;

    void drawCell(int x, int y, SDL_Color color);
};



#endif //RENDERER_HPP
