//
// Created by David Rynearson on 27.05.25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include "Board.hpp"
#include "Tetromino.hpp"

constexpr int CELL_SIZE = 30;                   // size of one block in pixels
constexpr int BOARD_PIX_W = BOARD_WIDTH * CELL_SIZE;
constexpr int BOARD_PIX_H = BOARD_HEIGHT * CELL_SIZE;

class Renderer {
public:
    Renderer(SDL_Renderer* ren, int offsetX, int offsetY, TTF_Font* fnt) : ren(ren), offsetX(offsetX), offsetY(offsetY), font(fnt) {}
    void drawBoard(const Board& board);
    void drawTetromino(const Tetromino& t);
    void drawScore(int score, int level, int lines);

private:
    SDL_Renderer* ren;
    int offsetX;
    int offsetY;
    TTF_Font* font;

    void drawCell(int x, int y, SDL_Color color);
};



#endif //RENDERER_HPP
